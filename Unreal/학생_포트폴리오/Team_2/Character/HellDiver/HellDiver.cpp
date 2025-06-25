// Fill out your copyright notice in the Description page of Project Settings.


#include "HellDiver.h"
#include "GameFramework/Character.h" // �̰� �ʿ���
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "HellDiverMovementComponent.h"
#include "HellDiverStateComponent.h"
#include "HellDiverStatComponent.h"
#include "HellDiverAnimInstance.h"
#include "PakourComponent.h"

#include "MotionWarpingComponent.h"
#include <Kismet/GameplayStaticsTypes.h>
#include <Kismet/GameplayStatics.h>
#include "Components/SplineMeshComponent.h"
#include "Components/SplineComponent.h"
#include "../../Object/Grenade/TimedGrenadeBase.h"
#include "../../Object/Stratagem/Stratagem.h"
#include "../../StratagemComponent.h"

#include "../StimPackComponent.h"

#include "../../Data/CollisionCameraDataAsset.h"

#include "../../Gun/GunBase.h"

AHellDiver::AHellDiver(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UHellDiverMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    GetCharacterMovement()->JumpZVelocity = 300.0f;

    _stateComponent = CreateDefaultSubobject<UHellDiverStateComponent>("State");


    _statComponent = CreateDefaultSubobject<UHellDiverStatComponent>("Stat");

    _stimPackComponent = CreateDefaultSubobject<UStimPackComponent>("StimPack");

    _stratagemComponent = CreateDefaultSubobject<UStratagemComponent>(TEXT("StratagemComponent"));

	_trajectorySpline = CreateDefaultSubobject<USplineComponent>(TEXT("ThrowSpline"));
    _trajectorySpline->SetupAttachment(GetMesh()); // �Ǵ� RootComponent

    _pakourComponent = CreateDefaultSubobject<UPakourComponent>(TEXT("PakourComponent"));
    _motionWarpComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpComponent"));


}

void AHellDiver::BeginPlay()
{
    Super::BeginPlay();
    SetCollisionState(_stateComponent->GetCharacterState());

    _stateComponent->_characterStateChanged.AddDynamic(this, &AHellDiver::SetCollisionState);
    auto anim = Cast<UHellDiverAnimInstance>(GetMesh()->GetAnimInstance());
    if (anim != nullptr)
    {
      
        anim->_moveChanged.AddDynamic(this->_stateComponent, &UHellDiverStateComponent::MoveChangeFinish);
        anim->_lookChanged.AddDynamic(this->_stateComponent, &UHellDiverStateComponent::LookChangeFinish);
    }

    if (_grenadeChanged.IsBound())
        _grenadeChanged.Broadcast(_curGrenade, _maxGrenade);
}

UHellDiverStateComponent* AHellDiver::GetStateComponent()
{
    return _stateComponent;
}

UHellDiverStatComponent* AHellDiver::GetStatComponent()
{
    return _statComponent;
}

UMotionWarpingComponent* AHellDiver::GetMotionWarp() const
{
    return _motionWarpComponent;
}

void AHellDiver::EquipGrenade()
{
	if (_heldThrowable || !_grenadeClass)
		return; // �̹� ����ִ�

    if (_curGrenade <= 0)
        return;

	GetStateComponent()->SetWeaponState(EWeaponType::Grenade);

	FActorSpawnParameters params;
	params.Owner = this;
	params.Instigator = this;

	FTransform spawnTransform = GetHandSocketTransform();
	_heldThrowable = GetWorld()->SpawnActor<ATimedGrenadeBase>(_grenadeClass, spawnTransform, params);

	if (_heldThrowable)
	{
		// �� ���Ͽ� ����
		_heldThrowable->AttachToHand(TEXT("hand_R")); // ���� �� �ִ� ������Ʈ���� �ñ�
	}
}

void AHellDiver::EquipStratagem()
{
	if (_heldThrowable)
		return;

	TSubclassOf<AStratagem> selectedStratagem = _stratagemComponent->GetSelectedStratagemClass();
	if (!selectedStratagem)
		return;

	GetStateComponent()->SetWeaponState(EWeaponType::StratagemDevice);

	FActorSpawnParameters params;
	params.Owner = this;
	params.Instigator = this;

	FTransform spawnTransform = GetHandSocketTransform();
	_heldThrowable = GetWorld()->SpawnActor<AStratagem>(selectedStratagem, spawnTransform, params);

	if (_heldThrowable)
	{
		_heldThrowable->AttachToHand(TEXT("hand_R"));
	}

}

void AHellDiver::OnThrowReleased()
{
	if (_heldThrowable)
	{
		FRotator throwRot = GetControlRotation();
		throwRot.Pitch += 20.0f; // ��ô ����
		FVector throwDirection = throwRot.Vector();

		_heldThrowable->Throw(throwDirection); // AThrowable ��� �Լ� ȣ��

        auto grenade = Cast<ATimedGrenadeBase>(_heldThrowable);
		_heldThrowable = nullptr;

        if (grenade) // ����ź�̸� �Ѱ� ����
        {
            _curGrenade--;

            if (_grenadeChanged.IsBound())
                _grenadeChanged.Broadcast(_curGrenade, _maxGrenade);
        }

		if (_stratagemComponent) // ���� ������ ��Ʈ��Ÿ�� ��� ��Ÿ�� ����
		{
			_stratagemComponent->CommitStratagemUse();
		}

		GetStateComponent()->SetWeaponState(EWeaponType::None);
	}
}

void AHellDiver::UpdateThrowSpline()
{
	if (!_heldThrowable || !_trajectorySpline)
		return;

	const FVector start = GetHandSocketTransform().GetLocation();
	const FRotator throwRot = GetControlRotation() + FRotator(20.f, 0.f, 0.f);
    const float power = GetStatComponent()->GetPower();
	const FVector velocity = throwRot.Vector() * power;

	FPredictProjectilePathParams params;
	params.StartLocation = start;
	params.LaunchVelocity = velocity;
	params.bTraceWithCollision = false;
	params.ProjectileRadius = 5.0f;
	params.MaxSimTime = 1.5f;
	params.SimFrequency = 15.f;
	params.OverrideGravityZ = -980.f; // Match gravity
	params.TraceChannel = ECC_Visibility;

	FPredictProjectilePathResult result;
    UGameplayStatics::PredictProjectilePath(GetWorld(), params, result);

	_trajectorySpline->ClearSplinePoints();

	for (const auto& point : result.PathData)
	{
		_trajectorySpline->AddSplinePoint(point.Location, ESplineCoordinateSpace::World);
	}
	_trajectorySpline->UpdateSpline();


    DrawThrowSplineMeshes();
}

void AHellDiver::ClearThrowSpline()
{
	// ���� �ð�ȭ ����
	for (USplineMeshComponent* mesh :_trajectoryMeshPool)
	{
		if (mesh)
		{
			mesh->DestroyComponent();
		}
	}
	_trajectoryMeshPool.Empty();

	// ����Ʈ�� ����
	if (_trajectorySpline)
	{
		_trajectorySpline->ClearSplinePoints();
	}
}

void AHellDiver::DrawThrowSplineMeshes()
{
	if (!_trajectoryMesh || !_trajectoryMaterial || !_trajectorySpline)
		return;

	// ���� ���� �޽� ����
	for (USplineMeshComponent* mesh : _trajectoryMeshPool)
	{
        if (mesh)
        {
            mesh->DestroyComponent();
        }
	}
	_trajectoryMeshPool.Empty();

	const int32 pointCount = _trajectorySpline->GetNumberOfSplinePoints();
    const int32 skipCount = 2; // ���� 2�� ������ �ð�ȭ ����
	for (int32 i = skipCount; i < pointCount - 1; ++i)
	{
		const FVector start = _trajectorySpline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector startTangent = _trajectorySpline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector end = _trajectorySpline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
		const FVector endTangent = _trajectorySpline->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

		USplineMeshComponent* mesh = NewObject<USplineMeshComponent>(this);
		mesh->SetMobility(EComponentMobility::Movable);
		mesh->AttachToComponent(_trajectorySpline, FAttachmentTransformRules::KeepRelativeTransform);
		mesh->RegisterComponent();
        mesh->SetCastShadow(false);
		mesh->SetStaticMesh(_trajectoryMesh);
		mesh->SetMaterial(0, _trajectoryMaterial);
		mesh->SetStartAndEnd(start, startTangent, end, endTangent);

		mesh->SetStartScale(FVector2D(0.2f, 0.2f));
		mesh->SetEndScale(FVector2D(0.2f, 0.2f));

		_trajectoryMeshPool.Add(mesh);
	}
}

void AHellDiver::StartThrowPreview()
{
    _isPreviewingThrow = true;
    if (_throwPreviewTimer.IsValid())
        return;
    GetWorldTimerManager().SetTimer(_throwPreviewTimer, this, &AHellDiver::UpdateThrowSpline, 0.05f, true);
}

void AHellDiver::StopThrowPreview()
{
    _isPreviewingThrow = false;

    GetWorldTimerManager().ClearTimer(_throwPreviewTimer);
    ClearThrowSpline();

    if (_heldThrowable)
    {
        _heldThrowable = nullptr;
    }
}

void AHellDiver::UseStimPack()
{
    _stimPackComponent->UseStimPack();
}

void AHellDiver::StartSprint()
{
    if (_stateComponent->StartSprint() == false)
        return;
    SetCollisionState(_stateComponent->GetCharacterState());
    auto movement=GetMovementComponent();
    GetCharacterMovement()->MaxWalkSpeed = _statComponent->GetSprintSpeed();
}

void AHellDiver::FinishSprint()
{
    if (_stateComponent->FinishSprint() == false)
        return;
    SetCollisionState(_stateComponent->GetCharacterState());

    auto movement = GetMovementComponent();
    GetCharacterMovement()->MaxWalkSpeed = _statComponent->GetDefaultSpeed();
}

void AHellDiver::StartCrouch()
{
    if (_stateComponent->StartCrouch() == false)
        return;
    SetCollisionState(_stateComponent->GetCharacterState());

    auto movement = GetMovementComponent();
    GetCharacterMovement()->MaxWalkSpeed = _statComponent->GetCrouchSpeed();
}

void AHellDiver::FinishCrouch()
{
    if (_stateComponent->FinishCrouch() == false)
        return;
    SetCollisionState(_stateComponent->GetCharacterState());

    auto movement = GetMovementComponent();
    GetCharacterMovement()->MaxWalkSpeed = _statComponent->GetDefaultSpeed();
}

void AHellDiver::StartProne()
{
    if (_stateComponent->StartProne() == false)
        return;
    SetCollisionState(_stateComponent->GetCharacterState());

    auto movement = GetMovementComponent();
    GetCharacterMovement()->MaxWalkSpeed = _statComponent->GetProneSpeed();
}

void AHellDiver::FinishProne()
{
    if (_stateComponent->FinishProne() == false)
        return;
    SetCollisionState(_stateComponent->GetCharacterState());
    auto movement = GetMovementComponent();
    GetCharacterMovement()->MaxWalkSpeed = _statComponent->GetDefaultSpeed();
}

void AHellDiver::Rolling()
{
    if (this->CanJump() == false)
        return;
    if (_stateComponent->IsRolling())
        return;
    if (_stateComponent->StartRolling()==false)
        return;
    Jump();

    FVector forward = GetActorForwardVector();

    float forwardBoost = 1000.0f; 
    FVector boost = forward * forwardBoost;

    // 4. ���� Velocity�� ������
    GetCharacterMovement()->Velocity += boost;

    StartProne();
}

void AHellDiver::FinishRolling()
{

    _stateComponent->FinishRolling();
}

void AHellDiver::Standing()
{
    GetCharacterMovement()->MaxWalkSpeed = _statComponent->GetDefaultSpeed();
}

void AHellDiver::Sprinting()
{
    GetCharacterMovement()->MaxWalkSpeed = _statComponent->GetSprintSpeed();
}

void AHellDiver::Crouching()
{
    GetCharacterMovement()->MaxWalkSpeed = _statComponent->GetCrouchSpeed();
}

void AHellDiver::Proning()
{
    GetCharacterMovement()->MaxWalkSpeed = _statComponent->GetProneSpeed();
}

AGunBase* AHellDiver::SpawnGun(TSubclassOf<AGunBase> gunClass)
{
    AGunBase* gun = GetWorld()->SpawnActor<AGunBase>(gunClass);
    gun->SetOwner(this);
    gun->InitializeGun();

    return gun;
}

void AHellDiver::EquipGun(AGunBase* gun)
{
    _equippedGun = gun;
    _equippedGun->ActivateGun();
    _stateComponent->SetWeaponState(EWeaponType::Gun);

    UE_LOG(LogTemp, Log, TEXT("Equip Gun"));
}

void AHellDiver::RefillAllItem()
{
    RefillMag();
    RefillGrenade();
    RefillStimPack();
}

void AHellDiver::RefillMag()
{
    for (auto gun : _gunSlot)
    {
        if (gun)
        {
            gun->RefillMag();
        }
    }
}

void AHellDiver::RefillGrenade()
{
    _curGrenade += 2;
    
    if (_curGrenade > _maxGrenade)
        _curGrenade = _maxGrenade;

    if (_grenadeChanged.IsBound())
        _grenadeChanged.Broadcast(_curGrenade, _maxGrenade);
}

void AHellDiver::RefillStimPack()
{
    _stimPackComponent->RefillStimPack();
}

void AHellDiver::MotionChangeFinish()
{

}

void AHellDiver::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);

    if (_stateComponent->IsRolling())
    {
        // ���� �ð� �� ����
        GetWorld()->GetTimerManager().SetTimer(
            _rollingTimerHandle, this, &AHellDiver::FinishRolling, 0.2, false
        );

    }


}

FRotator AHellDiver::Focusing()
{


    FRotator socketRot = GetMesh()->GetSocketRotation(TEXT("spine_05"));
    return socketRot;
}

float AHellDiver::TakeDamage(float damageAmount, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
    _statComponent->ChangeHp(-damageAmount);

    UE_LOG(LogTemp, Log, TEXT("Damage : %f"), damageAmount);

    if (_statComponent->IsDead())
        Dead();

    return -damageAmount;
}

FTransform AHellDiver::GetLeftHandSocketTransform() const
{
    if (_equippedGun ==nullptr)
    {
        return GetActorTransform(); // fallback
    }
    FTransform temp = _equippedGun->GetLeftHandleTrans();
    FVector resultLoc;
    FRotator resultRot;
    GetMesh()->TransformToBoneSpace(TEXT("hand_r"), temp.GetLocation(), temp.GetRotation().Rotator(), resultLoc,resultRot);
    temp.SetLocation(resultLoc);
    temp.SetRotation(resultRot.Quaternion());
    return temp ;
}

FTransform  AHellDiver::GetHandSocketTransform() const
{
	USkeletalMeshComponent* mesh = GetMesh();
	if (mesh && mesh->DoesSocketExist(TEXT("hand_R")))
	{
		return mesh->GetSocketTransform(TEXT("hand_R"));
	}
	return GetActorTransform(); // fallback
}

FTransform AHellDiver::GetEquip1SocketTransform() const
{
    USkeletalMeshComponent* mesh = GetMesh();
    if (mesh && mesh->DoesSocketExist(TEXT("EquipSocket_l")))
    {
        return mesh->GetSocketTransform(TEXT("EquipSocket_l"));
    }
    return GetActorTransform(); // fallback
}

FTransform AHellDiver::GetEquip2SocketTransform() const
{
    USkeletalMeshComponent* mesh = GetMesh();
    if (mesh && mesh->DoesSocketExist(TEXT("EquipSocket_r")))
    {
        return mesh->GetSocketTransform(TEXT("EquipSocket_r"));
    }
    return GetActorTransform(); // fallback
}

FTransform AHellDiver::GetEquip3SocketTransform() const
{
    USkeletalMeshComponent* mesh = GetMesh();
    if (mesh && mesh->DoesSocketExist(TEXT("EquipSocket_c")))
    {
        return mesh->GetSocketTransform(TEXT("EquipSocket_c"));
    }
    return GetActorTransform(); // fallback
}

void AHellDiver::SetCollisionState(ECharacterState newState)
{


    switch (newState)
    {
    case ECharacterState::Standing:
    case ECharacterState::Sprinting:
        SetStandingCollisionCamera();
        break;
    case ECharacterState::Crouching:
        SetCrouchingCollisionCamera();
        break;
    case ECharacterState::Proning:
    case ECharacterState::knockdown:
        SetProningCollisionCamera();
        break;
    case ECharacterState::MAX:
    default:
        break;
    }

}

void AHellDiver::SetCollisionCamera(UCollisionCameraDataAsset data)
{
    

}

void AHellDiver::SetStandingCollisionCamera()
{
    GetCapsuleComponent()->SetCapsuleRadius(_standingStance->_capsuleRadius);
    GetCapsuleComponent()->SetCapsuleHalfHeight(_standingStance->_capsuleHalfHeight);
    GetMesh()->SetRelativeLocation(FVector(0, 0, -_standingStance->_capsuleHalfHeight));
}

void AHellDiver::SetCrouchingCollisionCamera()
{
    GetCapsuleComponent()->SetCapsuleRadius(_crouchingStance->_capsuleRadius);
    GetCapsuleComponent()->SetCapsuleHalfHeight(_crouchingStance->_capsuleHalfHeight);
    GetMesh()->SetRelativeLocation(FVector(0, 0, -_crouchingStance->_capsuleHalfHeight));
    
}

void AHellDiver::SetProningCollisionCamera()
{
    GetCapsuleComponent()->SetCapsuleRadius(_proningStance->_capsuleRadius);
    GetCapsuleComponent()->SetCapsuleHalfHeight(_proningStance->_capsuleHalfHeight);
    GetMesh()->SetRelativeLocation(FVector(0, 0, -_proningStance->_capsuleHalfHeight));
}
