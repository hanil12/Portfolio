// Fill out your copyright notice in the Description page of Project Settings.

#include "Throwable.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "../Character/HellDiver/HellDiver.h"
#include "../Character/HellDiver/HellDiverStatComponent.h"

// Sets default values
AThrowable::AThrowable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = _mesh;


	_projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));	// �� ������Ʈ�� ����ü�� ���� ��� �̵��� �߷�, �ݻ� ���� �ڵ����� ó���մϴ�
	_projectileMovement->bRotationFollowsVelocity = true;	// ����ü�� �̵� ����(�ӵ� ����)�� ���� ȸ���ϵ��� �����մϴ�
	_projectileMovement->ProjectileGravityScale = 1.0f;	// �߷��� ������ �޴� ������ �����մϴ� (1.0 = �⺻ �߷�)
	_projectileMovement->InitialSpeed = 800.0f;	// ����ü�� ������ �� �ʱ� �ӵ��Դϴ�
	_projectileMovement->MaxSpeed = 3000.0f;	// ����ü�� ���� �� �ִ� �ִ� �ӵ��Դϴ� (�ӵ� ����)
	_projectileMovement->bShouldBounce = true;	// �浹 �� �ݻ�(ƨ��)�� Ȱ��ȭ�մϴ�
	_projectileMovement->Bounciness = 0.3f;	// ƨ���� �ݻ� �����Դϴ� (1.0 = ���� �ݻ�, 0.0 = �ݻ� ����)
	_projectileMovement->Friction = 0.3f;	// ���� ����Դϴ� (0 = �̲�����, 1 = �ſ� �� ����)
}

// Called when the game starts or when spawned
void AThrowable::BeginPlay()
{
	Super::BeginPlay();

	if (_mesh)
	{
		_mesh->OnComponentHit.AddDynamic(this, &AThrowable::OnHit);
		_mesh->SetNotifyRigidBodyCollision(true);
	}

	_owner = Cast<AHellDiver>(GetOwner());
	
}

// Called every frame
void AThrowable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AThrowable::AttachToHand(FName socketName)
{
	if (_owner)
	{
		if (USkeletalMeshComponent* characterMesh = _owner->GetMesh())
		{
			AttachToComponent(characterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, socketName);

			// ���� & �浹 ��Ȱ��ȭ
			if (GetMesh())
			{
				GetMesh()->SetSimulatePhysics(false);
				GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}

			if (_projectileMovement)
			{
				_projectileMovement->StopMovementImmediately();
				_projectileMovement->Deactivate();
			}
		}
	}
}

void AThrowable::Throw(FVector direction)
{
	if (!_owner || !_projectileMovement)
		return;

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	if (GetMesh())
	{
		//GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->IgnoreActorWhenMoving(_owner, true); // �����ڿ� �浹���� �߰�

		FTimerHandle collisionRecoveryTimer;
		GetWorldTimerManager().SetTimer(collisionRecoveryTimer, FTimerDelegate::CreateLambda([this]()
			{
				if (_owner && GetMesh())
				{
					GetMesh()->IgnoreActorWhenMoving(_owner, false);
				}
			}), 0.2f, false); // 0.2���� �����ڿ� �浹 ����
	}

	if (_projectileMovement)
	{
		_projectileMovement->Activate();
	}

	float power = _owner->GetStatComponent()->GetPower();

	_projectileMovement->Velocity = direction * power;

	GetWorldTimerManager().SetTimer(_destroyTimerHandle, this, &AThrowable::OnLifeTimeExpired, _lifeTime, false);

}

void AThrowable::OnLifeTimeExpired()
{
	DestroySelf();
}

void AThrowable::DestroySelf()
{
	if (_owner && _owner->GetHeldThrowable() == this) // �տ� ��� �ִ� ����ź�� ��츸
	{
		_owner->StopThrowPreview();
	}

	GetWorldTimerManager().ClearTimer(_destroyTimerHandle);
	Destroy();
}

void AThrowable::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this || OtherActor == _owner)
		return; // ����: �ڱ� �ڽ� �Ǵ� ������
}

