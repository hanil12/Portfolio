// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitalPrecisionStrike.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AOrbitalPrecisionStrike::AOrbitalPrecisionStrike()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = _mesh;

	_projectile = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile"));
	_projectile->ProjectileGravityScale = 0.0f;
	_projectile->InitialSpeed = 0.f;
	_projectile->MaxSpeed = 10000.f;
	_projectile->bRotationFollowsVelocity = true;
}

// Called when the game starts or when spawned
void AOrbitalPrecisionStrike::BeginPlay()
{
	Super::BeginPlay();

	_mesh->OnComponentHit.AddDynamic(this, &AOrbitalPrecisionStrike::OnHit);
	_mesh->SetNotifyRigidBodyCollision(true);
}

// Called every frame
void AOrbitalPrecisionStrike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AOrbitalPrecisionStrike::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::ApplyRadialDamage(
		GetWorld(),               // 1. ���� ���ؽ�Ʈ
		_damage,                  // 2. ������ ��ġ
		GetActorLocation(),       // 3. ���� �߽� ��ġ
		_radius,                  // 4. ���� �ݰ�
		nullptr,                  // 5. ������ Ÿ�� (�⺻ ������)
		{},                       // 6. ������ ���͵�
		this,                     // 7. �������� ������ ���� (���� �ڱ� �ڽ�)
		GetInstigatorController(),// 8. ������ ��Ʈ�ѷ� (�÷��̾� or AI)
		true                      // 9. �ݰ� �� ��� ���� ������
	);

	if (_explosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), _explosionEffect, GetActorLocation());
	}

	Destroy();
}

