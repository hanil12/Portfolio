// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "StatComponent.h"

// Sets default values
ACharacterBase::ACharacterBase(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ACharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterBase::UpDown(float value)
{
	if (abs(value) < 0.01f)
	{
		return;
	}
	_vertical = value;
	FRotator yawOnly = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	FVector forward = FRotationMatrix(yawOnly).GetUnitAxis(EAxis::X); // ���� ����

	AddMovementInput(forward * value * 10.0f);
}

void ACharacterBase::RightLeft(float value)
{
	if (abs(value) < 0.01f)
	{
		return;
	}
	_horizontal = value;
	FRotator yawOnly = FRotator(0.f, GetControlRotation().Yaw, 0.f);
	FVector right = FRotationMatrix(yawOnly).GetUnitAxis(EAxis::Y); // ���� ����

	AddMovementInput(right * value * 10.0f);
}


// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	float zVelocity = GetCharacterMovement()->Velocity.Z;

	UE_LOG(LogTemp, Log, TEXT("Landing Z Velocity: %f"), zVelocity);

	if (zVelocity < -1200.f)
	{
		// �ϵ巣��
	}
	else if (zVelocity < -400.f)
	{
		// �Ϲ� ����
	}
}

void ACharacterBase::KnockDown(float time)
{
	// �̵� ���߱�
	GetCharacterMovement()->DisableMovement();

	// ĸ���� �޽� �и� �浹 ó��
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));

	// ���� �ùķ��̼� ����
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->bBlendPhysics = true;

	// ���� �ð� �� ����
	GetWorld()->GetTimerManager().SetTimer(
		_knockDownTimerHandle, this, &ACharacterBase::KnockDownRecovery, time, false
	);
}

void ACharacterBase::KnockDownRecovery()
{
	// ���� �ùķ��̼� ����
	GetMesh()->SetSimulatePhysics(false);
	GetMesh()->SetAllBodiesSimulatePhysics(false);
	GetMesh()->bBlendPhysics = false;

	// �޽ø� ĸ���� ������
	FVector MeshLocation = GetMesh()->GetComponentLocation();
	FRotator MeshRotation = GetMesh()->GetComponentRotation();
	SetActorLocation(MeshLocation);
	SetActorRotation(FRotator(0, MeshRotation.Yaw, 0)); // ���� ���⸸ ����

	// �޽� ��ġ ����
	GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, NAME_None);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator::ZeroRotator);

	// �浹 �� �̵� ���� : ���߿� Ȯ���ϰ� �ٽ� Ȯ��.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

float ACharacterBase::TakeDamage(float damageAmount, FDamageEvent const& damageEvent, AController* eventInstigator, AActor* damageCauser)
{
	return -damageAmount;
}

void ACharacterBase::Dead()
{
	SetLifeSpan(3.0f);
}

