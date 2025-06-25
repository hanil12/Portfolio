// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Components/CapsuleComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"

#include "Engine/DamageEvents.h"
#include "Engine/OverlapResult.h"

#include "Components/SphereComponent.h"
#include "../Object/Item/ItemBase.h"

#include "../Gun/GunBase.h"
#include "../UI/UIManager.h"
#include "../UI/GunWidget.h"
#include "../UI/GunSettingWidget.h"
#include "../UI/StratagemWidget.h"

#include "../Object/Grenade/TimedGrenadeBase.h"
#include "../Object/Stratagem/Stratagem.h"
#include "../StratagemComponent.h"

#include "HellDiver/HellDiver.h"
#include "HellDiver/HellDiverStateComponent.h"
#include "HellDiver/PakourComponent.h"
#include "HellDiver/HellDiverStatComponent.h"

#include "../Data/PlayerControlDataAsset.h"
#include "../Data/CollisionCameraDataAsset.h"

#include "../Controller/MainPlayerController.h"
#include "../Controller/CameraContainActor.h"

#include "StimPackComponent.h"


APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	_cameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));

	_tpsSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPSSpringArm"));
	_tpsZoomSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPSZoomSpringArm"));
	_fpsSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("FPSSpringArm"));
	_tpsCameraActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("TPSCamera")) ;
	_tpsZoomCameraActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("TPSZoomCamera")) ;
	_fpsCameraActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("FPSCamera")) ;


	SetRootComponent(GetCapsuleComponent());
	GetMesh()->SetupAttachment(RootComponent);
	_cameraRoot->SetupAttachment(RootComponent); 
	//_cameraRoot->SetupAttachment(GetMesh(), FName("CameraSocket"));
	_tpsSpringArm->SetupAttachment(_cameraRoot);
	_tpsZoomSpringArm->SetupAttachment(_cameraRoot);
	_fpsSpringArm->SetupAttachment(_cameraRoot);


	_tpsCameraActor->SetupAttachment(_tpsSpringArm);
	_tpsZoomCameraActor->SetupAttachment(_tpsZoomSpringArm);
	_fpsCameraActor->SetupAttachment(_fpsSpringArm);
	_tpsCameraActor->SetChildActorClass(ACameraContainActor::StaticClass());
	_tpsZoomCameraActor->SetChildActorClass(ACameraContainActor::StaticClass());
	_fpsCameraActor->SetChildActorClass(ACameraContainActor::StaticClass());

	_itemDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("ItemDetectionSphere"));
	_itemDetectionSphere->SetupAttachment(RootComponent);
	_itemDetectionSphere->SetSphereRadius(100.f);
	_itemDetectionSphere->SetGenerateOverlapEvents(true);


}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	_gunSlot.SetNum(3);

	if (_gunWidgetClass)
	{
		_gunWidget = CreateWidget<UGunWidget>(GetWorld(), _gunWidgetClass);
	}

	if (_stgWidgetClass)
	{
		_stratagemWidget = CreateWidget<UStratagemWidget>(GetWorld(), _stgWidgetClass);
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetDefaultVIew();
	//InitView();
	SetTPSView();
	if (_gunClass1 && _gunClass2)
	{
		// �ӽ� ����
		_gunSlot[0] = SpawnGun(_gunClass1);
		_gunSlot[1] = SpawnGun(_gunClass2);
		_gunSlot[2] = SpawnGun(_gunClass3);

		EquipGun(_gunSlot[0]);
	}

	if (_gunWidget)
	{
		_equippedGun->_ammoChanged.AddUObject(_gunWidget, &UGunWidget::SetAmmo);
		_equippedGun->_magChanged.AddUObject(_gunWidget, &UGunWidget::SetMag);
		_statComponent->_hpChanged.AddUObject(_gunWidget, &UGunWidget::SetHp);
		_stimPackComponent->_stimPackChanged.AddUObject(_gunWidget, &UGunWidget::SetStimPack);
		_grenadeChanged.AddUObject(_gunWidget, &UGunWidget::SetGrenade);

		_gunWidget->AddToViewport();

		_equippedGun->ActivateGun();
		_stimPackComponent->BroadcastStimPackChanged();
		_gunWidget->SetGun(_equippedGun->GetGunData()._icon);
		if (_grenadeChanged.IsBound())
			_grenadeChanged.Broadcast(_curGrenade, _maxGrenade);
	}

	if (_stratagemWidget)
	{
		_stratagemWidget->InitializeWidget(_stratagemComponent->GetStratagemSlots());
		_stratagemWidget->AddToViewport();
		_stratagemWidget->OpenWidget(false);
	}

	//if (_sceneUIClass)
	//	UI->GetOrShowSceneUI(_sceneUIClass);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_stratagemComponent && _stratagemWidget)
	{
		for (int32 i = 0; i < _stratagemComponent->GetStratagemSlots().Num(); ++i)
		{
			if (_stratagemComponent->IsStratagemOnCooldown(i))
			{
				float remaining = _stratagemComponent->GetRemainingCooldown(i);
				_stratagemWidget->SetWidgetCooldownState(i, remaining);
			}
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent)
	{
		enhancedInputComponent->BindAction(_moveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		enhancedInputComponent->BindAction(_moveAction, ETriggerEvent::Completed, this, &APlayerCharacter::MoveFinish);
		enhancedInputComponent->BindAction(_lookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		enhancedInputComponent->BindAction(_jumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::TryJump);
		enhancedInputComponent->BindAction(_sprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::TrySprint);
		enhancedInputComponent->BindAction(_sprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);
		enhancedInputComponent->BindAction(_crouchAction, ETriggerEvent::Started, this, &APlayerCharacter::TryCrouch);
		enhancedInputComponent->BindAction(_proneAction, ETriggerEvent::Started, this, &APlayerCharacter::TryProne);
		enhancedInputComponent->BindAction(_rollingAction, ETriggerEvent::Started, this, &APlayerCharacter::TryRolling);
		enhancedInputComponent->BindAction(_testingViewAction, ETriggerEvent::Started, this, &APlayerCharacter::TryChangeControl);
		enhancedInputComponent->BindAction(_mouseLButtonAction, ETriggerEvent::Started, this, &APlayerCharacter::StartFiring);
		enhancedInputComponent->BindAction(_mouseLButtonAction, ETriggerEvent::Triggered, this, &APlayerCharacter::WhileFiring);
		enhancedInputComponent->BindAction(_mouseLButtonAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopFiring);
		enhancedInputComponent->BindAction(_mouseRButtonAction, ETriggerEvent::Started, this, &APlayerCharacter::StartAiming);
		enhancedInputComponent->BindAction(_mouseRButtonAction, ETriggerEvent::Triggered, this, &APlayerCharacter::WhileAiming);
		enhancedInputComponent->BindAction(_mouseRButtonAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopAiming);
		enhancedInputComponent->BindAction(_reloadAction, ETriggerEvent::Started, this, &APlayerCharacter::HoldReload);
		enhancedInputComponent->BindAction(_reloadAction, ETriggerEvent::Completed, this, &APlayerCharacter::ReleaseReload);
		enhancedInputComponent->BindAction(_weapon1ChangeAction, ETriggerEvent::Started, this, &APlayerCharacter::SwitchWeapon1);
		enhancedInputComponent->BindAction(_weapon2ChangeAction, ETriggerEvent::Started, this, &APlayerCharacter::SwitchWeapon2);
		enhancedInputComponent->BindAction(_weapon3ChangeAction, ETriggerEvent::Started, this, &APlayerCharacter::SwitchWeapon3);
		enhancedInputComponent->BindAction(_grenadeAction, ETriggerEvent::Triggered, this, &AHellDiver::EquipGrenade);
		enhancedInputComponent->BindAction(_lightChangeAction, ETriggerEvent::Started, this, &APlayerCharacter::TryChangeLightMode);
		enhancedInputComponent->BindAction(_scopeChangeAction, ETriggerEvent::Started, this, &APlayerCharacter::TryChangeScopeMode);
		enhancedInputComponent->BindAction(_aimChangeAction, ETriggerEvent::Started, this, &APlayerCharacter::ChangeAimingView);
		enhancedInputComponent->BindAction(_strataInputModeAction, ETriggerEvent::Started, this, &APlayerCharacter::BeginStratagemInputMode);
		enhancedInputComponent->BindAction(_strataInputModeAction, ETriggerEvent::Completed, this, &APlayerCharacter::EndStratagemInputMode);
		enhancedInputComponent->BindAction(_strataWAction, ETriggerEvent::Started, this, &APlayerCharacter::OnStrataKeyW);
		enhancedInputComponent->BindAction(_strataAAction, ETriggerEvent::Started, this, &APlayerCharacter::OnStrataKeyA);
		enhancedInputComponent->BindAction(_strataSAction, ETriggerEvent::Started, this, &APlayerCharacter::OnStrataKeyS);
		enhancedInputComponent->BindAction(_strataDAction, ETriggerEvent::Started, this, &APlayerCharacter::OnStrataKeyD);
		enhancedInputComponent->BindAction(_interactAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
		enhancedInputComponent->BindAction(_stimPackAction, ETriggerEvent::Started, this, &APlayerCharacter::OnUseStimPack);
	}
}

FTransform APlayerCharacter::GetLeftHandPos()
{

	return FTransform();
}


FRotator APlayerCharacter::Focusing()
{
	//���¿� ���� ������ �Ǵ� ���� �ٲ����.
	FTransform SpineTransform; 
	switch (_stateComponent->GetWeaponState())
	{
	case EWeaponType::None:
	case EWeaponType::Grenade:
	case EWeaponType::StratagemDevice:

		SpineTransform = GetMesh()->GetSocketTransform(TEXT("FocusingSocket"), RTS_World);


		break;
	case EWeaponType::Gun:
		SpineTransform = GetMesh()->GetSocketTransform(TEXT("weapon_r_muzzle"), RTS_World);
		break;
	default:
		break;

	}
	const FVector SpineLoc = SpineTransform.GetLocation();

	FVector CameraLoc, CameraForward;
	if (APlayerController* controller = Cast<APlayerController>(GetController()))
	{
		CameraLoc = controller->PlayerCameraManager->GetCameraLocation();
		CameraForward = controller->PlayerCameraManager->GetCameraRotation().Vector().GetSafeNormal();
	}
	else
	{
		CameraLoc = GetCurCamera()->GetComponentLocation();
		CameraForward = GetCurCamera()->GetComponentRotation().Vector().GetSafeNormal();
	}

	FVector AimTarget = CameraLoc + CameraForward * 10000.f;
	FVector SpineFwd = SpineTransform.GetRotation().Vector().GetSafeNormal();
	FVector SpineUp = SpineTransform.GetRotation().GetUpVector();
	FVector TargetDirection = (AimTarget - SpineLoc).GetSafeNormal();

	// �¿�(Yaw) �ʿ伺 �Ǵ�
	FVector SpineFwdFlat = FVector::VectorPlaneProject(SpineFwd, SpineUp).GetSafeNormal();
	FVector TargetDirFlat = FVector::VectorPlaneProject(TargetDirection, SpineUp).GetSafeNormal();
	float YawDot = FVector::DotProduct(SpineFwdFlat, TargetDirFlat);
	bool bNeedsYaw = YawDot < 0.999f;

	// ����(Pitch) �ʿ伺 �Ǵ�
	FVector SpineRight = SpineTransform.GetRotation().GetRightVector();
	FVector SpineFwdNoYaw = FVector::VectorPlaneProject(SpineFwd, SpineRight).GetSafeNormal();
	FVector TargetDirNoYaw = FVector::VectorPlaneProject(TargetDirection, SpineRight).GetSafeNormal();
	float PitchDot = FVector::DotProduct(SpineFwdNoYaw, TargetDirNoYaw);
	bool bNeedsPitch = PitchDot < 0.999f;

	// ��ü ȸ�� �Ǵ� (Roll�� ������ ����)
	float DotValue = FVector::DotProduct(SpineFwd, TargetDirection);
	FVector CrossValue = FVector::CrossProduct(SpineFwd, TargetDirection);
	float RotationDir = FVector::DotProduct(CrossValue, SpineUp);

	FRotator ResultRot = FRotator::ZeroRotator;

	if (DotValue > 0.999f) // ���� ��ġ�ϸ� ���� ȸ������ ����
	{
		return FRotator::ZeroRotator;
	}

	ResultRot.Roll = 1.4f - DotValue;

	if (bNeedsYaw)
	{
		ResultRot.Yaw = 1.0f * (RotationDir >= 0 ? 1.f : -1.f);
	}
	if (bNeedsPitch)
	{
		ResultRot.Pitch = 1.0f * ((SpineFwd.Z <= TargetDirection.Z) ? 1.f : -1.f);
	}

	//UE_LOG(LogTemp, Display, TEXT("pitch : %f Yaw : %f Roll : %f Dot : %f"), ResultRot.Pitch, ResultRot.Yaw, ResultRot.Roll, DotValue);
	return ResultRot;
}




void APlayerCharacter::Move(const FInputActionValue& value)
{
	if (_playerState == EPlayerState::StratagemInputting) // ��Ʈ��Ÿ���Է� ��忡���� ���۾���
		return;

	FVector2D moveVector = value.Get<FVector2D>();

	if (Controller != nullptr && moveVector.Length() > 0.01f)
	{
		if (_stateComponent->IsFocusing())
		{
			FocusMove(moveVector);
		}
		else
		{
			DefaultMove(moveVector);

		}


		//FVector MoveDirection = FVector(moveVector.X, moveVector.Y, 0.0f);
		////X�� �̵��������� ĳ���͸�ȸ���ϱ����� ȸ�� ��Ʈ���� ����. 
		//GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
		//
		////�̵�
		//AddMovementInput(MoveDirection, 1);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("MoveFinish"));
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		// ���ߴ� ���
		_vertical = 0.0f;
		_horizontal = 0.0f;

	}
}
void APlayerCharacter::MoveFinish(const FInputActionValue& value)
{
	if (_stateComponent->IsFocusing())
	{

	}
	else
	{

		ViewTurnBack();
		// ���ߴ� ���
		_vertical = 0.0f;
		_horizontal = 0.0f;

	}
}
void APlayerCharacter::Look(const FInputActionValue& value)
{
	FVector2D lookAxisVector = value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(lookAxisVector.X);
		AddControllerPitchInput(lookAxisVector.Y);

		_deltaAngle = FMath::FindDeltaAngleDegrees(GetActorRotation().Yaw, GetControlRotation().Yaw);
		if (_isViewTurnCenter&&_deltaAngle*_deltaAngle<=0.01)
		{
			_isViewTurnCenter = false;
			GetCharacterMovement()->bUseControllerDesiredRotation = false;

		}
		const bool bIsMoving = GetVelocity().Size2D() > 1.0f;
		//���̹��� �ƴϰ� ����� �ƴϿ�����.
		if (!_stateComponent->IsFocusing()&&bIsMoving)
		{
			//UE_LOG(LogTemp, Display, TEXT("MovingLook"));
			MovingLook();
		}
		
		else
		{
			//UE_LOG(LogTemp, Display, TEXT("DefaultLook"));
			DefaultLook(); // ������ �� ��90�� �Ѵ� ȸ�� ó��
		}
	}
}


void APlayerCharacter::TryJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		UE_LOG(LogTemp, Display, TEXT("TriggerPakour"));
		_pakourComponent->TriggerPakour();
	}
}

void APlayerCharacter::StartFiring(const FInputActionValue& value)
{
	switch (_stateComponent->GetCharacterState())
	{
	case ECharacterState::Sprinting:
		FinishSprint();
		break;
	case ECharacterState::Standing:
	case ECharacterState::Crouching:
	case ECharacterState::Proning:
	case ECharacterState::knockdown:
	case ECharacterState::MAX:
	default:
		break;
	}
	if (GetCharacterMovement()->bOrientRotationToMovement==true)
	{
		ViewTurnBack();
	}
	switch (_stateComponent->GetWeaponState())
	{
	case EWeaponType::Gun:
		if (_isGunSettingMode)
			return;

		_stateComponent->SetFiring(true);
		_equippedGun->StartFire();
		break;

	case EWeaponType::Grenade:
		_stateComponent->SetCookingGrenade(true);
		Cast<ATimedGrenadeBase>(_heldThrowable)->StartCookingGrenade();
		StartThrowPreview();
		break;

	case EWeaponType::StratagemDevice:
		_stateComponent->SetInputtingStratagem(true);
		StartThrowPreview();
		break;
	}

	/*switch (_playerState)
	{
	case EPlayerState::Idle:
		if (GetStateComponent()->GetWeaponState() == EWeaponType::PrimaryWeapon)
		{
			_playerState = EPlayerState::Firing;
			_stateComponent->SetFiring(true);
			_equippedGun->StartFire();
		}
		else if (GetStateComponent()->GetWeaponState() == EWeaponType::Grenade)
		{
			_playerState = EPlayerState::CookingGrenade;
			_equippedGrenade->StartCookingGrenade();
			StartThrowPreview();
		}
		else if (GetStateComponent()->GetWeaponState() == EWeaponType::StratagemDevice)
		{
			StartThrowPreview();
		}

		break;

	case EPlayerState::Firing:
		break;

	case EPlayerState::CookingGrenade:
		break;

	case EPlayerState::StratagemInputting:
		if (GetStateComponent()->GetWeaponState() == EWeaponType::StratagemDevice)
		{

		}
		break;

	case EPlayerState::Rolling:
		break;

	case EPlayerState::Reloading:
		break;
	}*/
}

void APlayerCharacter::WhileFiring(const FInputActionValue& value)
{
	if (_stateComponent->IsCookingGrenade())
		if (_heldThrowable)
			Cast<ATimedGrenadeBase>(_heldThrowable)->UpdateCookingGrenade();

	/*switch (_playerState)
	{
	case EPlayerState::Idle:
		break;

	case EPlayerState::Firing:
		break;

	case EPlayerState::CookingGrenade:
		if(_equippedGrenade)
			_equippedGrenade->UpdateCookingGrenade();
		break;

	case EPlayerState::StratagemInputting:
		break;

	case EPlayerState::Rolling:
		break;

	case EPlayerState::Reloading:
		break;
	}*/
}

void APlayerCharacter::StopFiring(const FInputActionValue& value)
{
	if (_stateComponent->IsFiring())
	{
		_stateComponent->SetFiring(false);
		_equippedGun->StopFire();
		return;
	}
	else if (_stateComponent->IsCookingGrenade())
	{
		_stateComponent->SetCookingGrenade(false);
		OnThrowReleased();
		StopThrowPreview();
		return;
	}
	else if (_stateComponent->IsInputtingStratagem())
	{
		_stateComponent->SetInputtingStratagem(false);
		OnThrowReleased();
		StopThrowPreview();
		return;
	}

	/*switch (_playerState)
	{
	case EPlayerState::Idle:
		if (GetStateComponent()->GetWeaponState() == EWeaponType::StratagemDevice)
		{
			OnThrowReleased();
			StopThrowPreview();
		}
		break;

	case EPlayerState::Firing:
		_playerState = EPlayerState::Idle;
		_stateComponent->SetFiring(false);
		_equippedGun->StopFire();
		break;

	case EPlayerState::CookingGrenade:
		_playerState = EPlayerState::Idle;
		OnThrowReleased();
		StopThrowPreview();
		break;

	case EPlayerState::StratagemInputting:
		_playerState = EPlayerState::Idle;
		OnThrowReleased();
		StopThrowPreview();
		break;

	case EPlayerState::Rolling:
		break;

	case EPlayerState::Reloading:
		break;
	}*/
}

void APlayerCharacter::TrySprint(const FInputActionValue& value)
{
	if (_viewType!=ECharacterViewType::TPS)
		return;
	switch (_stateComponent->GetCharacterState())
	{

	case ECharacterState::Standing:
		StartSprint();
		break;
	case ECharacterState::Sprinting:
		_pakourComponent->TriggerPakour();
		break;
	case ECharacterState::Crouching:
		FinishCrouch();
		break;
	case ECharacterState::Proning:
		FinishProne();
		break;
	case ECharacterState::knockdown:
	case ECharacterState::MAX:
	default:
		break;
	}
}
void APlayerCharacter::StartAiming(const FInputActionValue& value)
{
	if (_isGunSettingMode)
	{
		TryChangeFireMode(value);
		return;
	}

	if (GetCharacterMovement()->bOrientRotationToMovement == true)
	{
		ViewTurnBack();
	}
	if (_stateComponent->GetCharacterState() == ECharacterState::Sprinting)
		FinishSprint();
	_stateComponent->SetAiming(true);
	SetTPSZoomView();
	switch (_stateComponent->GetWeaponState())
	{
	case EWeaponType::Gun:
		_equippedGun->StartAiming();
		break;

	case EWeaponType::Grenade:
		break;

	case EWeaponType::StratagemDevice:
		break;

	case EWeaponType::None:

		break;
	}
}

void APlayerCharacter::StopSprint(const FInputActionValue& value)
{
	switch (_stateComponent->GetCharacterState())
	{

	case ECharacterState::Sprinting:
		FinishSprint();
		break;
	case ECharacterState::Standing:
	case ECharacterState::Crouching:
	case ECharacterState::Proning:
	case ECharacterState::knockdown:
	case ECharacterState::MAX:
	default:
		break;
	}
}
void APlayerCharacter::WhileAiming(const FInputActionValue& value)
{
	if (_isGunSettingMode)
		return;

	switch (_stateComponent->GetWeaponState())
	{
	case EWeaponType::Gun:
		break;

	case EWeaponType::Grenade:
		break;

	case EWeaponType::StratagemDevice:
		break;

	case EWeaponType::None:
		break;
	}
}

void APlayerCharacter::TryCrouch(const FInputActionValue& value)
{
	switch (_stateComponent->GetCharacterState())
	{

	case ECharacterState::Standing:
	case ECharacterState::Proning:
		StartCrouch();
		break;
	case ECharacterState::Crouching:
		FinishCrouch();
		break;
	case ECharacterState::Sprinting:
	case ECharacterState::knockdown:
	case ECharacterState::MAX:
	default:
		break;
	}
}

void APlayerCharacter::TryChangeControl(const FInputActionValue& value)
{

	if (_viewType == ECharacterViewType::TPS)
	{
		SetTPSZoomView();

	}
	else if (_viewType == ECharacterViewType::TPSZoom)
	{
		SetFPSView();

	}
	else if (_viewType == ECharacterViewType::FPS)
	{
		SetTPSView();
	}


}

void APlayerCharacter::TryProne(const FInputActionValue& value)
{
	switch (_stateComponent->GetCharacterState())
	{

	case ECharacterState::Standing:
	case ECharacterState::Crouching:
		StartProne();
		break;
	case ECharacterState::Proning:
		FinishProne();
		break;
	case ECharacterState::Sprinting:
	case ECharacterState::knockdown:
	case ECharacterState::MAX:
	default:
		break;
	}
}

void APlayerCharacter::TryRolling(const FInputActionValue& value)
{


	switch (_stateComponent->GetCharacterState())
	{

	case ECharacterState::Standing:
	case ECharacterState::Sprinting:
	case ECharacterState::Crouching:
		Rolling();
		break;
	case ECharacterState::Proning:
		FinishProne();
		break;
	case ECharacterState::knockdown:
	case ECharacterState::MAX:
	default:
		break;
	}
}
void APlayerCharacter::SetDefaultVIew()
{

	//���� ���� ����
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	//ĳ���� �����Ʈ ����
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0, 360, 0);

	_tpsSpringArm->bUsePawnControlRotation = true;
	_tpsZoomSpringArm->bUsePawnControlRotation = true;
	_fpsSpringArm->bUsePawnControlRotation = true;
	_tpsSpringArm->bInheritPitch		= true;
	_tpsSpringArm->bInheritYaw			= true;
	_tpsSpringArm->bInheritRoll			= true;
	_tpsZoomSpringArm->bInheritPitch	= true;
	_tpsZoomSpringArm->bInheritYaw		= true;
	_tpsZoomSpringArm->bInheritRoll		= true;
	_fpsSpringArm->bInheritPitch		= true;
	_fpsSpringArm->bInheritYaw			= true;
	_fpsSpringArm->bInheritRoll			= true;
}

void APlayerCharacter::SetMovingView()
{

	_tpsSpringArm->bUsePawnControlRotation = _defaultControl->bUsePawnContolRotation;
}

void APlayerCharacter::FinishMoving()
{
}

void APlayerCharacter::InitView()
{


	//ī�޶�
	if (_defaultControl != nullptr)
	{
		_tpsSpringArm->bUsePawnControlRotation = _defaultControl->bUsePawnContolRotation;
		_tpsSpringArm->bDoCollisionTest = _defaultControl->bDoCollisionTest;
		_tpsSpringArm->bInheritPitch = _defaultControl->bInheritPitch;
		_tpsSpringArm->bInheritYaw = _defaultControl->bInheritYaw;
		_tpsSpringArm->bInheritRoll = _defaultControl->bInheritRoll;

	}
	//ī�޶�
	if (_tpsControl != nullptr)
	{
		_tpsZoomSpringArm->bUsePawnControlRotation = _tpsControl->bUsePawnContolRotation;
		_tpsZoomSpringArm->bDoCollisionTest = _tpsControl->bDoCollisionTest;
		_tpsZoomSpringArm->bInheritPitch = _tpsControl->bInheritPitch;
		_tpsZoomSpringArm->bInheritYaw = _tpsControl->bInheritYaw;
		_tpsZoomSpringArm->bInheritRoll = _tpsControl->bInheritRoll;

	}
	//1��Ī
	if (_fpsControl != nullptr)
	{
		_fpsSpringArm->bUsePawnControlRotation =	_fpsControl->bUsePawnContolRotation;
		_fpsSpringArm->bDoCollisionTest =			_fpsControl->bDoCollisionTest;
		_fpsSpringArm->bInheritPitch =				_fpsControl->bInheritPitch;
		_fpsSpringArm->bInheritYaw =				_fpsControl->bInheritYaw;
		_fpsSpringArm->bInheritRoll =				_fpsControl->bInheritRoll;

	}

}
void APlayerCharacter::SetFPSView()
{
	if (_fpsControl == nullptr)
		return;
	//SetViewData(_fpsControl);
	_viewType = ECharacterViewType::FPS;
	ChangeViewCamera(_viewType);

}
void APlayerCharacter::SetTPSZoomView()
{
	if (_tpsControl == nullptr)
		return;
	//SetViewData(_tpsControl);
	_viewType = ECharacterViewType::TPSZoom;
	ChangeViewCamera(_viewType);
}
void APlayerCharacter::SetTPSView()
{
	if (_defaultControl == nullptr)
		return;
	//SetViewData(_defaultControl);
	_viewType = ECharacterViewType::TPS;
	ChangeViewCamera(_viewType);
}
void APlayerCharacter::SetViewData(const UPlayerControlDataAsset* characterControlData)
{

	//���� ���� ����
	bUseControllerRotationPitch = characterControlData->bUseControlRotationPitch;
	bUseControllerRotationYaw = characterControlData->bUseControlRotationYaw;
	bUseControllerRotationRoll = characterControlData->bUseControlRotationRoll;


	//ĳ���� �����Ʈ ����
	GetCharacterMovement()->bOrientRotationToMovement = characterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = characterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = characterControlData->RotationRate;






}
void APlayerCharacter::FocusMove(FVector2D moveVector)
{
	if (moveVector.SquaredLength() > 1.0f)
	{
		moveVector.Normalize();
	}

	const FRotator controlRot = Controller->GetControlRotation();
	const FRotator yawRotation(0, controlRot.Yaw, 0);

	const FVector forward = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	const FVector right = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

	_vertical = moveVector.Y;
	_horizontal = moveVector.X;

	AddMovementInput(forward, moveVector.Y);
	AddMovementInput(right, moveVector.X);
}

void APlayerCharacter::DefaultMove(FVector2D moveVector)
{
	if (moveVector.SquaredLength() > 1.0f)
	{
		moveVector.Normalize();
	}

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	const FRotator controlRot = Controller->GetControlRotation();
	const FRotator yawRotation(0.f, controlRot.Yaw, 0.f);

	const FVector forward = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	const FVector right = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

	const FVector desiredMoveDir = forward * moveVector.Y + right * moveVector.X;

	if (!desiredMoveDir.IsNearlyZero())
	{
		AddMovementInput(desiredMoveDir.GetSafeNormal(), 1.0f);

		const FVector localMoveDir = GetActorTransform().InverseTransformVectorNoScale(desiredMoveDir);
		_vertical = localMoveDir.X;
		_horizontal = localMoveDir.Y;
	}
	else
	{


		ViewTurnBack();
		// ���ߴ� ���
		_vertical = 0.0f;
		_horizontal = 0.0f;
	}
}

void APlayerCharacter::MovingLook()
{
	const FVector velocity = GetVelocity();
	if (velocity.Size2D() < 1.0f) return; // ���� ������ ȸ�� �� ��

	const FRotator controlRot = Controller->GetControlRotation();
	const FRotator yawRotation(0, controlRot.Yaw, 0);

	const FVector forward = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	const FVector right = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);

	// ���� �Է� ���� �������� �̵� ���� ����
	FVector moveDir = forward * _vertical + right * _horizontal;

	if (!moveDir.IsNearlyZero())
	{
		FRotator moveRot = moveDir.Rotation();
		SetActorRotation(FRotator(0.f, moveRot.Yaw, 0.f)); // ��� ȸ��
	}
}
void APlayerCharacter::DefaultLook()
{
	const float speed = GetVelocity().Size2D();
	const FRotator actorRot = GetActorRotation();
	const FRotator controlRot = GetControlRotation();

	if (_stateComponent->GetCharacterState() == ECharacterState::Proning)
	{
		//UE_LOG(LogTemp, Error, TEXT("ProningLook"));

		return;
	}

	if ( FMath::Abs(_deltaAngle) > 80.0f||GetCharacterMovement()->Velocity.Size() > 0.01f )
	{
		float targetYaw = FMath::RoundToFloat(controlRot.Yaw / 90.f) * 90.f;
		UE_LOG(LogTemp, Error, TEXT("DeltaAngle :%f"),_deltaAngle);

		_isTurnLeft = (_deltaAngle < -80.0f);
		_isTurnRight = (_deltaAngle > 80.0f);
		GetCharacterMovement()->bUseControllerDesiredRotation = true;

	}
	else if (FMath::Abs(_deltaAngle) < 1.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("DeltaAngle :%f"), _deltaAngle);
		_isTurnLeft = false;
		_isTurnRight = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
	}
}


void APlayerCharacter::ChangeViewCamera(ECharacterViewType type)
{
	UChildActorComponent* temp=_tpsCameraActor;
	USpringArmComponent* temp2 = nullptr;
	switch (type)
	{
	case ECharacterViewType::TPS:
		temp = _tpsCameraActor;
		temp2 = _tpsSpringArm;
		_stateComponent->SetAiming(false);
		break;
	case ECharacterViewType::TPSZoom:
		temp = _tpsZoomCameraActor;
		temp2 = _tpsZoomSpringArm;
		
		_stateComponent->SetAiming(true);
		break;
	case ECharacterViewType::FPS:
		temp = _fpsCameraActor;
		temp2 = _fpsSpringArm;
		break;
	default:
		break;
	}
	temp->SetActive(true);
	temp2->SetActive(true);
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC && temp && temp->GetChildActor())
	{
		PC->SetViewTargetWithBlend(temp->GetChildActor(), _cameraBlendTime);
	}
	DeactiveAnotherCamera();

}
void APlayerCharacter::DeactiveAnotherCamera()
{
	switch (_viewType)
	{
	case ECharacterViewType::TPS:
		_tpsZoomCameraActor->SetActive(false);
		_tpsZoomSpringArm->SetActive(false);
		_fpsCameraActor->SetActive(false);
		_fpsSpringArm->SetActive(false);
		break;
	case ECharacterViewType::TPSZoom:
		_tpsCameraActor->SetActive(false);
		_tpsSpringArm->SetActive(false);
		_fpsCameraActor->SetActive(false);
		_fpsSpringArm->SetActive(false);
		break;
	case ECharacterViewType::FPS:
		_tpsCameraActor->SetActive(false);
		_tpsSpringArm->SetActive(false);
		_tpsZoomCameraActor->SetActive(false);
		_tpsZoomSpringArm->SetActive(false);
		break;
	case ECharacterViewType::MAX:
	default:
		break;
	}
}
UChildActorComponent* APlayerCharacter::GetCurCamera()
{
	UChildActorComponent* curCamera=_tpsCameraActor;
	switch (_viewType)
	{
	case ECharacterViewType::TPS:
		curCamera = _tpsCameraActor;
		break;
	case ECharacterViewType::TPSZoom:
		curCamera = _tpsZoomCameraActor;
		break;
	case ECharacterViewType::FPS:
		curCamera = _fpsCameraActor;
		break;
	case ECharacterViewType::MAX:
	default:
		break;
	}

	return curCamera;

}
void APlayerCharacter::UpdateCameraOcclusion()
{
	FVector CameraLocation = _camera->GetComponentLocation(); // or CustomCamera
	FVector HeadLocation = GetMesh()->GetSocketLocation("head") + FVector(0, 0, 10.f); // �߽� ����

	TArray<FHitResult> Hits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->LineTraceMultiByChannel(
		Hits,
		CameraLocation,
		HeadLocation,
		ECC_Visibility,
		Params
	);

	// ���� ������Ʈ ó��
	for (const FHitResult& Hit : Hits)
	{
		if (UPrimitiveComponent* Comp = Hit.GetComponent())
		{
			if (!_fadedComponents.Contains(Comp))
			{
				Comp->SetRenderCustomDepth(true); // �Ǵ� ���� ��Ƽ����� ��ü
				_fadedComponents.Add(Comp);
			}
		}
	}

	// ���� �����ӿ� �־����� ������ ���� �� ����
	for (int32 i = _fadedComponents.Num() - 1; i >= 0; --i)
	{
		if (!Hits.ContainsByPredicate([&](const FHitResult& Hit) { return Hit.GetComponent() == _fadedComponents[i]; }))
		{
			_fadedComponents[i]->SetRenderCustomDepth(false); // �Ǵ� ���� ��Ƽ����� ����
			_fadedComponents.RemoveAt(i);
		}
	}

}
void APlayerCharacter::ViewTurnBack()
{
	UE_LOG(LogTemp, Display, TEXT("TurnBack"));
	AddActorWorldRotation(FRotator(0, 1, 0));
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	// ���ߴ� ���
	_isViewTurnCenter = true;
}
void APlayerCharacter::SetStandingCollisionCamera()
{
	Super::SetStandingCollisionCamera();


	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = NAME_None;
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__; // ����ũ�� ID

	UKismetSystemLibrary::MoveComponentTo(
		_cameraRoot,
		_standingStance->_cameraOffset,
		_cameraRoot->GetRelativeRotation(),
		true, true,
		0.2f, false,
		EMoveComponentAction::Move,
		LatentInfo
	);


}
void APlayerCharacter::SetCrouchingCollisionCamera()
{
	Super::SetCrouchingCollisionCamera();
	
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = NAME_None;
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__; // ����ũ�� ID

	UKismetSystemLibrary::MoveComponentTo(
		_cameraRoot,
		_crouchingStance->_cameraOffset,
		_cameraRoot->GetRelativeRotation(),
		true, true,
		0.2f, false,
		EMoveComponentAction::Move,
		LatentInfo
	);
}
void APlayerCharacter::SetProningCollisionCamera()
{
	Super::SetProningCollisionCamera();

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = NAME_None;
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__; // ����ũ�� ID

	UKismetSystemLibrary::MoveComponentTo(
		_cameraRoot,
		_proningStance->_cameraOffset,
		_cameraRoot->GetRelativeRotation(),
		true, true,
		0.2f, false,
		EMoveComponentAction::Move,
		LatentInfo
	);
}

void APlayerCharacter::SwitchWeapon(int32 index, const FInputActionValue& value)
{
	if (index > 3 || index < 0)
		return;

	if (index != 3 && _gunSlot[index] == nullptr)
		return;

	if (_isGunSettingMode)
		return;

	if (_stateComponent->IsReloading())
		_equippedGun->CancelReload();

	bool wasAiming = _stateComponent->IsAiming();
	bool wasFiring = _stateComponent->IsFiring();
	//_playerState = EPlayerState::Idle;
	_stateComponent->SetAiming(false);
	_stateComponent->SetFiring(false);

	if (index == 3)
	{
		// Grenade
	}
	else
	{
		_equippedGun->_ammoChanged.RemoveAll(_gunWidget);
		_equippedGun->_magChanged.RemoveAll(_gunWidget);
		_equippedGun->DeactivateGun();

		EquipGun(_gunSlot[index]);

		_equippedGun->_ammoChanged.AddUObject(_gunWidget, &UGunWidget::SetAmmo);
		_equippedGun->_magChanged.AddUObject(_gunWidget, &UGunWidget::SetMag);
		_equippedGun->ActivateGun();
		
		if (_gunWidget)
			_gunWidget->SetGun(_equippedGun->GetGunData()._icon);
	}

	_stateComponent->SetEquipIndex(index);

	if (wasAiming) // ���� ���̾��� ��� ����
		StartAiming(value);

	if (wasFiring) // ��� ���̾��� ��� ����
		StartFiring(value);
}

void APlayerCharacter::BeginStratagemInputMode(const FInputActionValue& value)
{
	if (_playerState == EPlayerState::Idle)
	{
		_playerState = EPlayerState::StratagemInputting;
		_stratagemInputBuffer.Empty();
	}

	_stratagemWidget->OpenWidget(true);
}

void APlayerCharacter::EndStratagemInputMode(const FInputActionValue& value)
{
	if (_playerState == EPlayerState::StratagemInputting)
	{
		_playerState = EPlayerState::Idle;
		_stratagemInputBuffer.Empty(); // ���� �ʱ�ȭ
	}

	_stratagemWidget->OpenWidget(false);
}

void APlayerCharacter::OnStrataKeyW(const FInputActionValue& value)
{
	if (_playerState == EPlayerState::StratagemInputting)
	{
		_stratagemInputBuffer.Add(EKeys::W);
		CheckStratagemInputCombo();
	}
}

void APlayerCharacter::OnStrataKeyA(const FInputActionValue& value)
{
	if (_playerState == EPlayerState::StratagemInputting)
	{
		_stratagemInputBuffer.Add(EKeys::A);
		CheckStratagemInputCombo();
	}
}

void APlayerCharacter::OnStrataKeyS(const FInputActionValue& value)
{
	if (_playerState == EPlayerState::StratagemInputting)
	{
		_stratagemInputBuffer.Add(EKeys::S);
		CheckStratagemInputCombo();
	}
}

void APlayerCharacter::OnStrataKeyD(const FInputActionValue& value)
{
	if (_playerState == EPlayerState::StratagemInputting)
	{
		_stratagemInputBuffer.Add(EKeys::D);
		CheckStratagemInputCombo();
	}
}

void APlayerCharacter::OnUseStimPack(const FInputActionValue& value)
{
	UseStimPack();
}

void APlayerCharacter::CheckStratagemInputCombo()
{
	const TArray<FStratagemSlot>& slots = _stratagemComponent->GetStratagemSlots();
	bool bIsPrefixMatch = false;

	for (int32 i = 0; i < slots.Num(); ++i)
	{
		if (_stratagemComponent->IsStratagemOnCooldown(i))
			continue;

		TSubclassOf<AStratagem> stratagemClass = slots[i].StratagemClass;
		if (!stratagemClass) continue;

		const AStratagem* CDO = stratagemClass->GetDefaultObject<AStratagem>();
		const TArray<FKey>& combo = CDO->GetInputSequence();

		// ���� ��ġ �� ���
		if (_stratagemInputBuffer == combo)
		{
			_stratagemComponent->SelectStratagem(i);
			EquipStratagem();

			_stratagemInputBuffer.Empty();
			_playerState = EPlayerState::Idle;

			_stratagemWidget->SetWidgetOperatingState(i);
			return;
		}

		// ��� ������ ��Ʈ��Ÿ���� �ִ��� Ȯ��
		if (_stratagemInputBuffer.Num() <= combo.Num())
		{
			bool bPrefixMatch = true;
			for (int32 j = 0; j < _stratagemInputBuffer.Num(); ++j)
			{
				if (_stratagemInputBuffer[j] != combo[j])
				{
					bPrefixMatch = false;
					break;
				}
			}
			if (bPrefixMatch)
			{
				bIsPrefixMatch = true;
			}

			_stratagemWidget->UpdateWidget(i, _stratagemInputBuffer.Num(), bPrefixMatch);
		}
	}

	if (!bIsPrefixMatch)
	{
		_stratagemInputBuffer.Empty(); // ���� �ʱ�ȭ
		_stratagemWidget->OpenWidget(false);
	}
}

void APlayerCharacter::Interact(const FInputActionValue& value)
{
	TArray<AActor*> overlapped;
	_itemDetectionSphere->GetOverlappingActors(overlapped, AItemBase::StaticClass());

	// 1) ������ ��ģ ������(Ȥ�� ���� ���� ��ġ)�� ���ؼ��� �ٷ� �Ⱦ�
	for (AActor* actor : overlapped)
	{
		AItemBase* item = Cast<AItemBase>(actor);
		if (!item) continue;

		float dist = FVector::Dist(GetActorLocation(), item->GetActorLocation());
		if (dist <= KINDA_SMALL_NUMBER)
		{
			item->PickupItem(this);
			return;  // ���� ���� �߰ߵ� ��ģ �����۸� ó��
		}
	}

	// 2) ��ġ�� ���� �����۵鿡 ���� ���� ���ھ� ���� ����
	AItemBase* bestItem = nullptr; // ���� ������ ������ ������
	float bestScore = -1.0f; // �񱳿� ���ھ�(Ŭ���� �켱)
	const FVector forward = GetActorForwardVector().GetSafeNormal();
	const FVector playerLoc = GetActorLocation();

	for (AActor* actor : overlapped)
	{
		AItemBase* item = Cast<AItemBase>(actor);
		if (!item) continue;

		FVector toItem = item->GetActorLocation() - playerLoc;
		float dist = toItem.Size();

		FVector dir = toItem / dist;
		// �÷��̾��� ���� ���Ϳ� ������ ���� ������ ���� ���
		float forwardDot = FVector::DotProduct(forward, dir);
		float score = (forwardDot > 0.0f) ? (forwardDot / dist) : 0.0f;

		if (score > bestScore)
		{
			bestScore = score;
			bestItem = item;
		}
	}

	if (bestItem)
	{
		bestItem->PickupItem(this);
	}
}

void APlayerCharacter::StopAiming(const FInputActionValue& value)
{
	if (_isGunSettingMode)
		return;

	_stateComponent->SetAiming(false);
	SetTPSView();
	switch (_stateComponent->GetWeaponState())
	{
	case EWeaponType::Gun:
		_equippedGun->StopAiming();
		break;

	case EWeaponType::Grenade:
		break;

	case EWeaponType::StratagemDevice:
		break;

	case EWeaponType::None:
		break;
	}
}

void APlayerCharacter::HoldReload(const FInputActionValue& value)
{
	_reloadPressedTime = GetWorld()->GetTimeSeconds();
	_isGunSettingMode = false;

	GetWorldTimerManager().SetTimer(_gunSettingTimer, this, &APlayerCharacter::EnterGunSetting, 0.7f, false);
}

void APlayerCharacter::ReleaseReload(const FInputActionValue& value)
{
	GetWorldTimerManager().ClearTimer(_gunSettingTimer);

	if (_stateComponent->GetWeaponState() != EWeaponType::Gun)
		return;

	if (_equippedGun)
	{
		if (_isGunSettingMode)
		{
			UE_LOG(LogTemp, Log, TEXT("Exit Gun Setting"));
			
			GetGameInstance()->GetSubsystem<UUIManager>()->ClosePopUp("GunSetting");
			
			_isGunSettingMode = false;
			return;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Reload"));
			_equippedGun->StopAiming();
			_equippedGun->StopFire();
			_equippedGun->Reload();
			return;
		}
	}
}

void APlayerCharacter::EnterGunSetting()
{
	if (_stateComponent->IsAiming())
		return;

	if (_stateComponent->IsFiring())
		return;

	if (_stateComponent->GetWeaponState() != EWeaponType::Gun)
		return;

	if (_equippedGun)
	{
		_isGunSettingMode = true;

		if (_stateComponent->IsAiming())
			return;

		if (_stateComponent->IsFiring())
			return;

		if (auto widget = GET_WIDGET(UGunSettingWidget, "GunSetting"))
		{
			widget->InitializeWidget(_equippedGun);
		}

		UE_LOG(LogTemp, Log, TEXT("Enter Gun Setting"));
	}
}

void APlayerCharacter::TryChangeFireMode(const FInputActionValue& value)
{
	if (_stateComponent->IsAiming())
		return;

	if (_stateComponent->IsFiring())
		return;

	if (_stateComponent->GetWeaponState() != EWeaponType::Gun)
		return;

	if (_equippedGun && _isGunSettingMode)
	{
		_equippedGun->ChangeFireMode();
		if (auto widget = GET_WIDGET(UGunSettingWidget, "GunSetting"))
		{
			widget->UpdateFireModePanel(_equippedGun->GetCurFireMode());
		}
	}
}

void APlayerCharacter::TryChangeLightMode(const FInputActionValue& value)
{
	if (_stateComponent->IsAiming())
		return;

	if (_stateComponent->IsFiring())
		return;

	if (_stateComponent->GetWeaponState() != EWeaponType::Gun)
		return;

	if (_equippedGun && _isGunSettingMode)
	{
		_equippedGun->ChangeTacticalLightMode();

		if (auto widget = GET_WIDGET(UGunSettingWidget, "GunSetting"))
		{
			widget->UpdateLightModePanel(_equippedGun->GetCurLightMode());
		}
	}
}

void APlayerCharacter::TryChangeScopeMode(const FInputActionValue& value)
{
	if (_stateComponent->IsAiming())
		return;

	if (_stateComponent->IsFiring())
		return;

	if (_stateComponent->GetWeaponState() != EWeaponType::Gun)
		return;

	if (_equippedGun && _isGunSettingMode)
	{
		_equippedGun->ChangeScopeMode();
		if (auto widget = GET_WIDGET(UGunSettingWidget, "GunSetting"))
		{
			widget->UpdateScopeModePanel(_equippedGun->GetCurScopeMode());
		}
	}
}

void APlayerCharacter::ChangeAimingView(const FInputActionValue& value)
{
	UE_LOG(LogTemp, Log, TEXT("TryAimChange"));

	bool isAiming = _stateComponent->IsAiming();

	if (!isAiming) return;

	if (_viewType == ECharacterViewType::TPSZoom)
	{
		SetFPSView();
		return;
	}

	if (_viewType == ECharacterViewType::FPS)
	{
		SetTPSZoomView();
		return;
	}
}
