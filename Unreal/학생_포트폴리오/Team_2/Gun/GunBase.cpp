// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBase.h"

#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "../UI/ImpactMarker.h"
#include "Blueprint/UserWidget.h"

#include "../Character/HellDiver/HellDiver.h"
#include "../Character/HellDiver/HellDiverStateComponent.h"
#include "../Character/CharacterAnimInstance.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SpotLightComponent.h"

// Sets default values
AGunBase::AGunBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	RootComponent = _mesh;

	_tacticalLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
	_tacticalLight->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGunBase::BeginPlay()
{
	Super::BeginPlay();

	if (_crosshairClass)
	{
		_crosshair = CreateWidget<UUserWidget>(GetWorld(), _crosshairClass);

		if (_crosshair)
		{
			_crosshair->AddToViewport();
			_crosshair->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (_laserFX)
	{
		_laserpointer = UNiagaraFunctionLibrary::SpawnSystemAttached(
			_laserFX,
			_mesh,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}

	if (_laserImpactFX)
	{
		_laserImpact = UNiagaraFunctionLibrary::SpawnSystemAttached(
			_laserImpactFX,
			_mesh,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}

	_curAmmo = _gunData._maxAmmo;
	_curMag = _gunData._initialMag;
}

// Called every frame
void AGunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto camera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	if (!camera) return;
	
	RecoverRecoil(DeltaTime);
	FHitResult hitResult= GetHitResult();
	_hitPoint = hitResult.bBlockingHit ? hitResult.ImpactPoint : hitResult.TraceEnd;

	if (_laserpointer)
		UseLaserPoint(_hitPoint);

	if (!_isActive) return;

	if (!_owner->GetStateComponent()->IsAiming())
		return;

	if (!IsValid(_marker))
		_marker = GetWorld()->SpawnActor<AImpactMarker>(_impactMarkerClass, _hitPoint, FRotator::ZeroRotator);
	else
		_marker->SetActorLocation(_hitPoint);
}

void AGunBase::StartFire()
{
	if (!_owner)
		return;
	
	if(_owner->GetStateComponent()->IsReloading())
		return;

	_owner->GetStateComponent()->SetFiring(true);

	GetWorldTimerManager().ClearTimer(_fireTimer);

	switch (_fireMode)
	{
	case EFireMode::FireAuto: // ������ ���� �߻�
		GetWorldTimerManager().SetTimer(_fireTimer, this, &AGunBase::Fire, _gunData._fireInterval, true, 0.0f);
		break;
	case EFireMode::FireSemi: // �� ���� �߻�
	case EFireMode::FireBoltAction: // ��Ʈ�׼�
		Fire();
		StopFire();
		break;
	case EFireMode::FireBurst: // 3�� �߻�
		_burstCount = 3;
		GetWorld()->GetTimerManager().SetTimer(_fireTimer, this, &AGunBase::Fire, _gunData._fireInterval, true);
	}
}

void AGunBase::Fire()
{
	FColor drawColor = FColor::Green;
	
	// źâ, ��� ��� �����
	if (_curAmmo <= 0 && !_isChamberLoaded)
	{
		UE_LOG(LogTemp, Log, TEXT("Mag Empty"));
		StopFire();
		return;
	}
	
	auto camera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	if (!camera) return;

	if (_fireMode == EFireMode::FireBoltAction)
	{
		if (!_canFire) return;

		_canFire = false;
		GetWorldTimerManager().SetTimer(_boltActionTimer, this, &AGunBase::ResetCanFire, 1.2f, false);
	}

	if (_fireMode == EFireMode::FireBurst)
	{
		if (_burstCount <= 0)
		{
			StopFire();
			return;
		}
		_burstCount--;
	}

	ApplyFireRecoil();

	FHitResult hitResult = GetHitResult();
	/*ApplyFireRecoil();

	FRotator cameraRotation = camera->GetCameraRotation();
	FVector muzzleLocation;

	if (_mesh && _mesh->DoesSocketExist(TEXT("Muzzle")))
	{
		muzzleLocation = _mesh->GetSocketLocation(TEXT("Muzzle"));
	}
	else
	{
		muzzleLocation = GetActorLocation() + cameraRotation.Vector() * 100;
	}
	FVector start = muzzleLocation;

	// �÷��̾�Լ� ȭ�� �߾� ��������
	FVector dir = (_hitPoint - start).GetSafeNormal();
	
	// �������� ���� ��� ź����
	if (!_owner->GetStateComponent()->IsAiming())
	{
		dir = FMath::VRandCone(dir, FMath::DegreesToRadians(_gunData._recoil));
		_hitPoint = start + dir * 10000.0f; 
	}

	FHitResult hitResult;
	FCollisionQueryParams params(NAME_None, false, this);

	bool bResult = GetWorld()->LineTraceSingleByChannel(
		OUT hitResult,
		start,
		_hitPoint,
		ECC_Visibility,
		params);*/

	if (hitResult.bBlockingHit)
	{
		drawColor = FColor::Red;
		float distance = FVector::Dist(hitResult.TraceStart, hitResult.ImpactPoint);
		float finalDamage = CalculateDamage(distance / 100);

		if (ACharacterBase* character = Cast<ACharacterBase>(hitResult.GetActor()))
		{
			UGameplayStatics::ApplyDamage(character, finalDamage, _owner->GetController(), this, nullptr);
		}
	}

	if (_curAmmo > 0) // źâ�� ź���� �������� ���
	{
		_curAmmo--;
	}
	else // ��ǿ��� �������� ���
	{
		_isChamberLoaded = false;
	}
	
	if (_ammoChanged.IsBound())
		_ammoChanged.Broadcast(_curAmmo, _gunData._maxAmmo);

	_hitPoint = hitResult.bBlockingHit ? hitResult.ImpactPoint : hitResult.TraceEnd;
	DrawDebugLine(GetWorld(), hitResult.TraceStart, _hitPoint, drawColor, false, 1.0f);
}

void AGunBase::StopFire()
{
	if (_owner)
	{
		_owner->GetStateComponent()->SetFiring(false);
	}

	GetWorldTimerManager().ClearTimer(_fireTimer);
}

void AGunBase::StartAiming()
{
	if (_owner->GetStateComponent()->IsReloading())
		return;

	_owner->GetStateComponent()->SetAiming(true);

	if (_marker)
		_marker->SetActorHiddenInGame(false);
	if (_crosshair)
		_crosshair->SetVisibility(ESlateVisibility::Visible);

	if (_laserpointer && _laserImpact)
	{
		_laserpointer->SetVisibility(true);
		_laserImpact->SetVisibility(true);
	}

	if (_tacticalLight)
		UseTacticalLight(true);
}

void AGunBase::StopAiming()
{
	_owner->GetStateComponent()->SetAiming(false);

	if (_marker)
		_marker->SetActorHiddenInGame(true);
	if (_crosshair)
		_crosshair->SetVisibility(ESlateVisibility::Hidden);

	if (_laserpointer && _laserImpact)
	{
		_laserpointer->SetVisibility(false);
		_laserImpact->SetVisibility(false);
	}

	if (_tacticalLight)
		UseTacticalLight(false);
}

void AGunBase::InitializeGun()
{
	if (AHellDiver* owner = Cast<AHellDiver>(GetOwner()))
	{
		_owner = owner;
		AttachToHand();
		UE_LOG(LogTemp, Log, TEXT("Initialize Gun"));
	}

	DeactivateGun();
}

void AGunBase::ActivateGun()
{
	_isActive = true;
	SetActorHiddenInGame(false);

	_recoilToRecover = FRotator::ZeroRotator;

	if (_laserpointer && _laserImpact)
	{
		_laserpointer->SetVisibility(false);
		_laserImpact->SetVisibility(false);
	}

	if (_ammoChanged.IsBound())
	{
		_ammoChanged.Broadcast(_curAmmo, _gunData._maxAmmo);
	}

	if (_magChanged.IsBound())
	{
		_magChanged.Broadcast(_curMag, _gunData._maxMag);
	}
}

void AGunBase::DeactivateGun()
{
	StopFire();
	StopAiming();

	_isActive = false;
	SetActorHiddenInGame(true);

	if (_tacticalLight)
		UseTacticalLight(_owner->GetStateComponent()->IsAiming());

	if (_marker)
		_marker->SetActorHiddenInGame(true);

	if (_crosshair)
		_crosshair->SetVisibility(ESlateVisibility::Hidden);

	if (_laserpointer && _laserImpact)
	{
		_laserpointer->SetVisibility(false);
		_laserImpact->SetVisibility(false);
	}
}

void AGunBase::AttachToHand()
{
	if (_owner)
	{
		if (USkeletalMeshComponent* characterMesh = _owner->GetMesh())
		{
			AttachToComponent(characterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_R"));
			SetActorRelativeRotation(FRotator(0.f, 90.f, 0.f));

			// ���� & �浹 ��Ȱ��ȭ
			if (_mesh)
			{
				_mesh->SetSimulatePhysics(false);
				_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
}

void AGunBase::Reload() // �ִϸ��̼ǰ� ���� �ʿ�
{
	if (_curAmmo == _gunData._maxAmmo)
		return;

	if (_curMag == 0)
		return;

	_owner->GetStateComponent()->SetReloading(true);

	if (!_reloadMontage) return;

	if (UCharacterAnimInstance* animInstance = Cast<UCharacterAnimInstance>(_owner->GetMesh()->GetAnimInstance()))
	{
		_owner->GetStateComponent()->StartReload();
		animInstance->PlayAnimMontage(_reloadMontage);

		// ��� �� �ν��Ͻ� ��������
		if (FAnimMontageInstance* MontageInstance = animInstance->GetActiveInstanceForMontage(_reloadMontage))
		{
			// ��������Ʈ �ߺ� ����
			MontageInstance->OnMontageEnded.Unbind();

			// ��������Ʈ ���ε�
			MontageInstance->OnMontageEnded.BindUObject(this, &AGunBase::FinishReload);

			UE_LOG(LogTemp, Error, TEXT("Success to get MontageInstance for %s"), *_reloadMontage->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get MontageInstance for %s"), *_reloadMontage->GetName());
		}

		int32 sectionIndex = -1;
		switch (_reloadStage)
		{
		case EReloadStage::None:	
			if (_curMag <= 0) return;
			sectionIndex = 0;
			break;

		case EReloadStage::RemoveMag:
			sectionIndex = 1;
			break;

		case EReloadStage::InsertMag:
			sectionIndex = 2;
			break;

		case EReloadStage::CloseBolt:
			sectionIndex = 3;
			break;

		case EReloadStage::RoundsReload:
			if (_curMag <= 0) return;
			sectionIndex = 3;
			break;

		default:
			return;
		}

		if (sectionIndex >= 0)
		{
			animInstance->JumpToSection(sectionIndex);
		}

		
	}
}

void AGunBase::FinishReload(UAnimMontage* Montage, bool bInterrupted)
{
	_owner->GetStateComponent()->FinishReload();
}

void AGunBase::ChangeReloadStage()
{
	switch (_reloadStage)
	{
	case EReloadStage::None:
		_reloadStage = EReloadStage::RemoveMag;
		if (_curAmmo > 0 || _isChamberLoaded) // źâ�̳� ��ǿ� ź�� ���� ���
			_isChamberLoaded = true; // ��� ä���
		_curAmmo = 0;
		UE_LOG(LogTemp, Log, TEXT("None->RemoveMag"));
		Reload();
		break;

	case EReloadStage::RemoveMag: // źâ ���� ����
		_reloadStage = EReloadStage::InsertMag;
		_curMag--;
		Reload();
		UE_LOG(LogTemp, Log, TEXT("RemoveMag->InsertMag"));
		break;

	case EReloadStage::InsertMag: // źâ ���� ����
		if (_isChamberLoaded) // ��ǿ� ź�� ���� ��� 
		{
			_isChamberLoaded = true;
			_curAmmo = _gunData._maxAmmo;
			_owner->GetStateComponent()->SetReloading(false);
			_reloadStage = EReloadStage::None;  // ���� ������ -> CloseBolt ����
			UE_LOG(LogTemp, Log, TEXT("InsertMag->None"));
		}
		else
		{
			_reloadStage = EReloadStage::CloseBolt; // ��ǿ� ź�� ���� ��� CloseBolt
			Reload();
			UE_LOG(LogTemp, Log, TEXT("InsertMag->CloseBolt"));
		}
		break;

	case EReloadStage::CloseBolt:
		_curAmmo = _gunData._maxAmmo;
		_owner->GetStateComponent()->SetReloading(false);
		_reloadStage = EReloadStage::None;
		UE_LOG(LogTemp, Log, TEXT("CloseBolt->None"));
		break;

	case EReloadStage::RoundsReload:
		_curAmmo++;
		_curMag--;
		_owner->GetStateComponent()->SetReloading(false);
		Reload();
		UE_LOG(LogTemp, Log, TEXT("RoundsReload"));
		break;
	}

	if (_ammoChanged.IsBound())
	{
		_ammoChanged.Broadcast(_curAmmo, _gunData._maxAmmo);
	}

	if (_magChanged.IsBound())
		_magChanged.Broadcast(_curMag, _gunData._maxMag);
}

void AGunBase::CancelReload()
{
	if (!_owner->GetStateComponent()->IsReloading())
		return;

	//StopAnimMontage();
	_owner->GetStateComponent()->SetReloading(false);
}

void AGunBase::RefillMag()
{
	_curMag += _gunData._refillMagAmount;

	if (_curMag > _gunData._maxMag)
		_curMag = _gunData._maxMag;

	if (_magChanged.IsBound())
		_magChanged.Broadcast(_curMag, _gunData._maxMag);
}

float AGunBase::CalculateDamage(float distance)
{
	if (distance <= 25.0f)
	{
		float Alpha = distance / 25.0f;
		float Falloff = FMath::Lerp(0.0f, _gunData._falloff25, Alpha);
		return _gunData._baseDamage * (1.0f - Falloff);
	}
	else if (distance <= 50.0f)
	{
		float Alpha = (distance - 25.0f) / 25.0f;
		float Falloff = FMath::Lerp(_gunData._falloff25, _gunData._falloff50, Alpha);
		return _gunData._baseDamage * (1.0f - Falloff);
	}
	else if (distance <= 100.0f)
	{
		float Alpha = (distance - 50.0f) / 50.0f;
		float Falloff = FMath::Lerp(_gunData._falloff50, _gunData._falloff100, Alpha);
		return _gunData._baseDamage * (1.0f - Falloff);
	}
	else
	{
		return _gunData._baseDamage * (1.0f - _gunData._falloff100);
	}
}

//void AGunBase::TickRecoil(float DeltaTime)
//{
//	if (_owner->GetStateComponent()->IsFiring())
//		return;
//
//	float recoilMultiplier = GetRecoilMultiplier();
//
//	static float RecoilTime = 0.f;
//	RecoilTime += DeltaTime * 2.f * recoilMultiplier;
//
//	// �ӽð�
//	float amplitudePitch = 0.12f;
//	float amplitudeYaw = 0.04f;
//
//	_recoilOffset.Pitch += FMath::Sin(RecoilTime) * amplitudePitch * recoilMultiplier;
//	_recoilOffset.Yaw += FMath::Cos(RecoilTime / 2) * amplitudeYaw * recoilMultiplier;
//}

void AGunBase::RecoverRecoil(float DeltaTime)
{
	APlayerController* playerController = Cast<APlayerController>(_owner->GetController());
	if (playerController)
	{
		float recoverPitch = FMath::FInterpTo(_recoilToRecover.Pitch, 0.f, DeltaTime, _recoilRecoverSpeed);
		float recoverYaw = FMath::FInterpTo(_recoilToRecover.Yaw, 0.f, DeltaTime, _recoilRecoverSpeed);

		// ��ȭ���� �ݴ�� �ǵ���
		playerController->AddPitchInput(_recoilToRecover.Pitch - recoverPitch);
		playerController->AddYawInput(recoverYaw - _recoilToRecover.Yaw);

		_recoilToRecover.Pitch = recoverPitch;
		_recoilToRecover.Yaw = recoverYaw;
	}
}

void AGunBase::ApplyFireRecoil()
{
	float recoilMultiplier = GetRecoilMultiplier();

	// ���� �ݵ�
	float vertical = FMath::RandRange(0.9f, 1.1f) * _gunData._verticalRecoil / _verticalRecoilDamp;
	// ���� �ݵ�
	float horizontal = FMath::RandRange(-1.f, 1.f) * _gunData._horizontalRecoil / _horizontalRecoilDamp;

	APlayerController* playerController = Cast<APlayerController>(_owner->GetController());
	if (playerController)
	{
		playerController->AddPitchInput(-vertical * recoilMultiplier);
		playerController->AddYawInput(horizontal * recoilMultiplier);

		// ī�޶� ���� ��
		_recoilToRecover.Pitch += vertical * recoilMultiplier;
		_recoilToRecover.Yaw += horizontal * recoilMultiplier;
	}
}

float AGunBase::GetRecoilMultiplier()
{
	float base = 1.f;

	if (_owner)
	{
		FVector velocity = _owner->GetVelocity();

		// �����̰� ���� �ʴٸ�
		if (velocity.SizeSquared() < FMath::Square(10.f))
		{
			switch (_owner->GetStateComponent()->GetCharacterState())
			{
			case ECharacterState::Standing:
				base = 0.8f;
				break;
			case ECharacterState::Crouching:
				base = 0.6f;
				break;
			case ECharacterState::Proning:
				base = 0.4f;
				break;
			}
		}
		// �����̰� �ִٸ�
		else
		{
			switch (_owner->GetStateComponent()->GetCharacterState())
			{
			case ECharacterState::Standing:
				base = 1.5f;
				break;
			case ECharacterState::Crouching:
				base = 1.f;
				break;
			}
		}
	}
	return base;
}


FHitResult AGunBase::GetHitResult()
{
	//// ȭ�� �߾� ����
	//FVector cameraLocation;
	//FRotator cameraRotation;
	//GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(cameraLocation, cameraRotation);
	//
	//FVector start = cameraLocation;
	//
	//// ���� �� ����Ʈ���̽� �̿��Ͽ� ��Ŀ ��ġ ���
	//FHitResult hitResult;
	//FCollisionQueryParams params(NAME_None, false, this);
	//
	//FVector end = start + (cameraRotation + _recoilOffset).Vector() * 10000;
	//bool bResult = GetWorld()->LineTraceSingleByChannel(
	//	OUT hitResult,
	//	start,
	//	end,
	//	ECC_Visibility,
	//	params);
	//
	//if (bResult)
	//	return hitResult.Location;
	//else
	//	return end;

	FVector muzzleLocation = _mesh->GetSocketLocation(TEXT("Muzzle"));
	FVector muzzleDirection = _mesh->GetSocketRotation(TEXT("Muzzle")).Vector();

	FVector end = muzzleLocation + muzzleDirection * 10000.f;

	FHitResult hit;
	GetWorld()->LineTraceSingleByChannel(
		hit,
		muzzleLocation,
		end,
		ECC_Pawn);

	return hit;
}

void AGunBase::ChangeFireMode()
{
	if (_gunData._fireModes.Num() <= 1)
		return;

	_fireIndex = (_fireIndex + 1) % _gunData._fireModes.Num();
	_fireMode = _gunData._fireModes[_fireIndex];

	UE_LOG(LogTemp, Log, TEXT("Fire Mode Change"));
}

void AGunBase::ChangeTacticalLightMode()
{
	if (!_tacticalLight) return;

	switch (_tacticalLightMode)
	{
	case ETacticalLightMode::LightOn:
		_tacticalLightMode = ETacticalLightMode::LightOff;
		break;

	case ETacticalLightMode::LightOff:
		_tacticalLightMode = ETacticalLightMode::LightAuto;
		break;

	case ETacticalLightMode::LightAuto:
		_tacticalLightMode = ETacticalLightMode::LightOn;
		break;
	}

	UseTacticalLight(_owner->GetStateComponent()->IsAiming());
	UE_LOG(LogTemp, Log, TEXT("Light Mode Change"));
}

void AGunBase::ChangeScopeMode()
{
	if (_gunData._scopeModes.Num() <= 1)
		return;

	_scopeIndex = (_scopeIndex + 1) % _gunData._scopeModes.Num();
	_scopeMode = _gunData._scopeModes[_scopeIndex];

	UE_LOG(LogTemp, Log, TEXT("Fire Mode Change"));
}

void AGunBase::UseLaserPoint(FVector hitPoint)
{
	FVector start;

	if (_mesh && _mesh->DoesSocketExist(TEXT("LaserPoint")))
	{
		start = _mesh->GetSocketLocation(TEXT("LaserPoint"));
	}
	else
	{
		start = GetActorLocation();
	}

	FVector end = hitPoint;

	if (_laserpointer)
	{
		_laserpointer->SetVectorParameter("Beam Start", start);
		_laserpointer->SetVectorParameter("Beam End", end);
	}

	if (_laserImpact)
		_laserImpact->SetWorldLocation(end);
}

void AGunBase::UseTacticalLight(bool isAiming)
{
	if (!_tacticalLight) return;

	switch (_tacticalLightMode)
	{
	case ETacticalLightMode::LightOn:
		_tacticalLight->SetVisibility(true);
		break;

	case ETacticalLightMode::LightOff:
		_tacticalLight->SetVisibility(false);
		break;

	case ETacticalLightMode::LightAuto:
		_tacticalLight->SetVisibility(isAiming);
		break;
	}
}

FTransform AGunBase::GetMuzzleTrans()
{
	return  _mesh->GetSocketTransform(TEXT("Muzzle"),RTS_World);
}

FVector AGunBase::GetMuzzleLoc()
{
	return _mesh->GetSocketLocation(TEXT("Muzzle"));;
}

FRotator AGunBase::GetMuzzleRot()
{
	return _mesh->GetSocketRotation(TEXT("Muzzle"));
}

FTransform AGunBase::GetLeftHandleTrans()
{
	if(_mesh==nullptr)
		return GetActorTransform();
	if(_mesh->DoesSocketExist(TEXT("LeftGrip")))
		return _mesh->GetSocketTransform(TEXT("LeftGrip"),RTS_World);
	return _mesh->GetComponentTransform();
		
}

