// Fill out your copyright notice in the Description page of Project Settings.


#include "PakourComponent.h"
#include "Engine/DamageEvents.h"
#include "../../Helper/H_Vector.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/Character.h"
#include "HellDiver.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MotionWarpingComponent.h"
#include "Animation/AnimInstance.h"             // UAnimInstance, Montage_Play(), GetActiveInstanceForMontage()
#include "Animation/AnimMontage.h"              // UAnimMontage
// Sets default values for this component's properties
UPakourComponent::UPakourComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPakourComponent::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
	// ...
	
}

void UPakourComponent::ReadyPakour(UAnimMontage* Montage, bool bInterrupted)
{
	_canPakour = true;
	UE_LOG(LogTemp, Display, TEXT("Call ReadyPakour"));
}

void UPakourComponent::ActiveColNMove(UAnimMontage* Montage, bool bInterrupted)
{

	_capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	_movement->SetMovementMode(EMovementMode::MOVE_Walking);
	UE_LOG(LogTemp, Display, TEXT("Call ActiveColNMove"));
}

void UPakourComponent::DeactiveColNMove()
{

	_capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	_movement->SetMovementMode(EMovementMode::MOVE_Flying);
}


// Called every frame
void UPakourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPakourComponent::TriggerPakour()
{
	if (_canPakour == false)
		return;
	FVector hitLoc;
	FRotator normal;
	UE_LOG(LogTemp, Display, TEXT("TriggerPakour"));
	bool bResult=DetectWall(hitLoc, normal);
	if (bResult == false)
	{

		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Detect"));

	ScanWall(hitLoc, normal);

	UE_LOG(LogTemp, Display, TEXT("ScanFinish"));
	MeasureWall();
	UE_LOG(LogTemp, Display, TEXT("MeasureFinish"));
	TryPakour();
	UE_LOG(LogTemp, Display, TEXT("TryPakour"));
}

bool UPakourComponent::DetectWall(FVector& hitLocation, FRotator& normal)
{
	for (int i = 0; i < 8; i++)
	{
		//�ڽ��� ����������� �߿������� ���οö󰡸� ����������� ĸ�� �ݸ����� �ؼ� ���� �ִ��� Ž��. Ž���� ��Ȯ���ϱ����� ���� �ڿ������� ����.
		FVector temp = H_Vector::MoveVectorDownward(GetOwner()->GetActorLocation(), 60);
		temp = H_Vector::MoveVectorUpward(temp, i * 20);
		FVector end = temp + (GetOwner()->GetActorForwardVector() * 200.0f);
		FVector start= H_Vector::MoveVectorBackward(temp, GetOwner()->GetActorRotation(), 30);
		 //start���� ���� 200��ŭ �߰���.

		FHitResult hit;
		
		FCollisionQueryParams params(NAME_None, false, GetOwner());

		bool bResult=GetWorld()->SweepSingleByChannel(hit,
			start,
			end,
			FQuat::Identity,
			ECC_Visibility,
			FCollisionShape::MakeSphere(10),
			params);
		//ã������ ���� ��ġ�� ���� �������Ϳ� ����� ��ȯ.
		if (bResult)
		{
			hitLocation = hit.Location;
			normal = hit.Normal.Rotation();
			return true;
		}
	}
	return false;
}

void UPakourComponent::ScanWall(FVector detectLocation, FRotator reverseNormal)
{
	for (int i = 0; i < 30; i++)
	{
		//Ž���� ��ġ�κ��� ����� ���� ���������� �Ʒ��� 10�� �������鼭.
		FVector temp = H_Vector::MoveVectorUpward(detectLocation, 300);
		temp = H_Vector::MoveVectorDownward(temp, i * 10);
		//����������� 100��ŭ ����Ʈ���̽�
		FVector start = H_Vector::MoveVectorBackward(temp, reverseNormal, 20);
		FVector end=H_Vector::MoveVectorForward(temp,reverseNormal,80);

		FHitResult hit;

		FCollisionQueryParams params(NAME_None, false, GetOwner());

		bool bResult = GetWorld()->LineTraceSingleByChannel(hit,
			start,
			end,
			ECC_Visibility,
			params);
		//ã������ break
		if (bResult)
		{
			_facedWallHitResult = hit;

			//UE_LOG(LogTemp, Display, TEXT("Find _facedWallHitResult"));
			break;
		}
	}
	//���� ���� ���� �� �� �浹��ġ�� ������.
	_wallRotation = H_Vector::ReverseNormal(_facedWallHitResult.Normal);
	for (int i = 0; i < 7; i++)
	{

		//�浹�������� ������ 20������ ������ �Ʒ��� ���Ǿ� Ʈ���̽��� �ؼ� ���� �β��� Ȯ����.
		FVector temp=H_Vector::MoveVectorForward(_facedWallHitResult.Location,_wallRotation,i*20);
		FVector start = H_Vector::MoveVectorUpward(temp, 10);
		FVector end = H_Vector::MoveVectorDownward(start, 10);

		FHitResult hit;

		FCollisionQueryParams params(NAME_None, false, GetOwner());

		bool bResult = GetWorld()->SweepSingleByChannel(hit,
			start,
			end,
			FQuat::Identity,
			ECC_Visibility,
			FCollisionShape::MakeSphere(5),
			params);
		if (i == 0)
		{
			//ù �浹�� ���� ���� �浹����.
			if (bResult)
			{
				_firstTopHitResult = hit;

				//UE_LOG(LogTemp, Display, TEXT("Find _firstTopHitResult"));
			}

		}
		else
		{
			//��� ���ŵǸ� �浹�� �Ǹ� ����, �ȵǸ� �ݺ��� ���� . �� ���� �β��� ����������� Ȯ��.
			if (bResult)
			{
				//UE_LOG(LogTemp, Display, TEXT("Find _lastTopHitResult"));
				_lastTopHitResult = hit;
			}
			else
			{
				break;
			}
		}

	}

	{
		//���� ���������� ��¦ �����ΰ��� ���������� ���Ǿ�Ʈ���̽�
		FVector start = H_Vector::MoveVectorForward(_lastTopHitResult.ImpactPoint, _wallRotation, 20);
		FVector end = _lastTopHitResult.ImpactPoint;

		FHitResult hit;

		FCollisionQueryParams params(NAME_None, false, GetOwner());

		bool bResult = GetWorld()->SweepSingleByChannel(hit,
			start,
			end,
			FQuat::Identity,
			ECC_Visibility,
			FCollisionShape::MakeSphere(10),
			params);
		if (bResult==false)
		{

			//UE_LOG(LogTemp, Display, TEXT("Is not Wall"));
			return;
		}
		//�� ��ġ�� ���� ������ Ȯ��, ����ġ�� ����.
		_endOfWallHitResult = hit;

		//UE_LOG(LogTemp, Display, TEXT("Find _endOfWallHitResult"));
	}
	{
		//������ ��¦ �տ��� ������ ���Ǿ�Ʈ���̽�. ���������� �����.
		FVector start = H_Vector::MoveVectorForward(_endOfWallHitResult.ImpactPoint, _wallRotation, 60);
		FVector end = H_Vector::MoveVectorDownward(start, 100);

		FHitResult hit;

		FCollisionQueryParams params(NAME_None, false, GetOwner());

		bool bResult = GetWorld()->SweepSingleByChannel(hit,
			start,
			end,
			FQuat::Identity,
			ECC_Visibility,
			FCollisionShape::MakeSphere(10),
			params);
		if (!bResult)
		{
			//UE_LOG(LogTemp, Display, TEXT("Not Found VaultLandingPoint"));
			return;

		}
		_vaultLandingHitResult = hit;
		//UE_LOG(LogTemp, Display, TEXT("Find _vaultLandingHitResult"));
	}
	

	
}

void UPakourComponent::MeasureWall()
{
	//�� ���̸� ����. ���� ����� ���� �浹������ �ڽ��� ���̸� �A ��.
	if (_firstTopHitResult.bBlockingHit && _facedWallHitResult.bBlockingHit)
	{
		if (_ownerCharacter)
		{

			_wallHeight = _firstTopHitResult.ImpactPoint.Z - _ownerCharacter->GetMesh()->GetComponentLocation().Z;
		}
	}
	else
	{
		_wallHeight = 0.0f;

	}

	//UE_LOG(LogTemp, Display, TEXT("wallHeight : %f"),_wallHeight);
}

void UPakourComponent::Initialize()
{
	_ownerCharacter = Cast<AHellDiver>(GetOwner());
	_capsule = _ownerCharacter->GetCapsuleComponent();
	_mesh = _ownerCharacter->GetMesh();
	_motionWarp = _ownerCharacter->GetMotionWarp();
	_movement = _ownerCharacter->GetCharacterMovement();
	_montageEnded.BindUObject(this, &UPakourComponent::ReadyPakour);
	_montageBlendOutStarted.BindUObject(this, &UPakourComponent::ActiveColNMove);
}

void UPakourComponent::TryPakour()
{
	if (_firstTopHitResult.bBlockingHit == false)
	{
		return;
	}
	//������������
	if (_movement->IsMovingOnGround() == false)
	{
		return;

	}
	if (_wallHeight > 300.0f)
	{
		//Too High Wall
		return;
	}
	if (_wallHeight > 130)
	{
		//Climb;
		return;
	}
	if (FMath::IsNearlyEqual(_ownerCharacter->GetVelocity().Size2D(), 0.0f, 5.0f))
	{
		//Mantle
		return;
	}
	if (_wallHeight > 100.0f)
	{
		//FrontFlip
		TryVault(EVaultType_C::FrontFlip);
		return;
	}
	if (_wallHeight > 90)
	{
		//TwoHandVault
		TryVault(EVaultType_C::TwoHandVault);

		return;

	}
	if (_wallHeight > 60)
	{
		//OneHandVault
		TryVault(EVaultType_C::OneHandVault);

		return;

	}
	else
	{

		return;
	}
}

void UPakourComponent::TryVault(EVaultType_C type)
{
	_canPakour = false;

	_capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	_movement->SetMovementMode(EMovementMode::MOVE_Flying);

	UAnimInstance* animInstance = _mesh->GetAnimInstance();
	if (!animInstance) return;

	UAnimMontage* selectedMontage = nullptr;

	switch (type)
	{
	case EVaultType_C::OneHandVault:
		UE_LOG(LogTemp, Display, TEXT("OneHandVault Height: %f"), _wallHeight);
		selectedMontage = _oneHandVault;
		break;

	case EVaultType_C::TwoHandVault:
	{
		UE_LOG(LogTemp, Display, TEXT("TwoHandVault Height: %f"), _wallHeight);
		UE_LOG(LogTemp, Display, TEXT("FirstTopHit: %f : %f : %f"), _firstTopHitResult.Location.X, _firstTopHitResult.Location.Y, _firstTopHitResult.Location.Z);
		UE_LOG(LogTemp, Display, TEXT("VaultLanding: %f : %f : %f"), _vaultLandingHitResult.Location.X, _vaultLandingHitResult.Location.Y, _vaultLandingHitResult.Location.Z);
		UE_LOG(LogTemp, Display, TEXT("Actor Rotation: %f : %f : %f"), GetOwner()->GetActorRotation().Pitch, GetOwner()->GetActorRotation().Yaw, GetOwner()->GetActorRotation().Roll);
		UE_LOG(LogTemp, Display, TEXT("wall Rotation: %f : %f : %f"), _wallRotation.Pitch, _wallRotation.Yaw, _wallRotation.Roll);
		selectedMontage = _twoHandVault;
		FVector temp = H_Vector::MoveVectorForward(_firstTopHitResult.Location, _wallRotation, 62);
		temp = H_Vector::MoveVectorDownward(temp, 46.5);
		FRotator rot = _wallRotation;
		rot.Roll = 0;
		_motionWarp->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("VaultStart"),temp, rot);
		temp = _vaultLandingHitResult.Location;
		_motionWarp->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("VaultEnd"),temp, rot);

	}
		break;

	case EVaultType_C::FrontFlip:
	{
		UE_LOG(LogTemp, Display, TEXT("FrontFlip Height: %f"), _wallHeight);
		UE_LOG(LogTemp, Display, TEXT("FirstTopHit: %f : %f : %f"), _firstTopHitResult.Location.X, _firstTopHitResult.Location.Y, _firstTopHitResult.Location.Z);
		UE_LOG(LogTemp, Display, TEXT("VaultLanding: %f : %f : %f"), _vaultLandingHitResult.Location.X, _vaultLandingHitResult.Location.Y, _vaultLandingHitResult.Location.Z);
		UE_LOG(LogTemp, Display, TEXT("Actor Rotation: %f : %f : %f"), GetOwner()->GetActorRotation().Pitch, GetOwner()->GetActorRotation().Yaw, GetOwner()->GetActorRotation().Roll);
		UE_LOG(LogTemp, Display, TEXT("wall Rotation: %f : %f : %f"),_wallRotation.Pitch, _wallRotation.Yaw, _wallRotation.Roll);
		selectedMontage = _frontFlip;
		FVector temp = H_Vector::MoveVectorBackward(_firstTopHitResult.Location, _wallRotation, 100);
		FRotator rot = _wallRotation;
		rot.Roll = 0;
		_motionWarp->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("VaultStart"), temp, rot);
		FVector temp2 = H_Vector::MoveVectorRight(_vaultLandingHitResult.Location, _wallRotation, 5);
		_motionWarp->AddOrUpdateWarpTargetFromLocationAndRotation(TEXT("VaultEnd"), temp2, rot);
		break;

	}

	default:
		return;
	}

	if (!selectedMontage) return;

	// �ִϸ��̼� ���
	const float Duration = animInstance->Montage_Play(selectedMontage);
	if (Duration <= 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to play montage: %s"), *selectedMontage->GetName());
		return;
	}

	// ��� �� �ν��Ͻ� ��������
	if (FAnimMontageInstance* MontageInstance = animInstance->GetActiveInstanceForMontage(selectedMontage))
	{
		// ��������Ʈ �ߺ� ����
		MontageInstance->OnMontageEnded.Unbind();
		MontageInstance->OnMontageBlendingOutStarted.Unbind();

		// ��������Ʈ ���ε�
		MontageInstance->OnMontageEnded.BindUObject(this, &UPakourComponent::ReadyPakour);
		MontageInstance->OnMontageBlendingOutStarted.BindUObject(this, &UPakourComponent::ActiveColNMove);

		UE_LOG(LogTemp, Display, TEXT("Delegates bound for %s"), *selectedMontage->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get MontageInstance for %s"), *selectedMontage->GetName());
	}
}


