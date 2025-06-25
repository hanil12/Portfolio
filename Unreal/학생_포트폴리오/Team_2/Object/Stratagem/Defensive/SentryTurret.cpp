// Fill out your copyright notice in the Description page of Project Settings.

#include "SentryTurret.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h" 
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ASentryTurret::ASentryTurret()
{
	PrimaryActorTick.bCanEverTick = true;
	// ���������������������� �ͷ� ����(Static Mesh) ����������������������
	_bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	RootComponent = _bodyMesh;
	_bodyMesh->SetCollisionProfileName(TEXT("BlockAll"));

	// ���������������������� �ͷ� �ѱ�(Static Mesh) ����������������������
	_barrelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	_barrelMesh->SetupAttachment(_bodyMesh);
	_barrelMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	_curAmmo = 0;

	// ���� �÷��� ������Ʈ
	_muzzleFlashComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MuzzleFlashComponent"));
	_muzzleFlashComponent->SetupAttachment(_barrelMesh);
	_muzzleFlashComponent->bAutoActivate = false;
	// �����Ϳ��� �ش� ������Ʈ�� Template(ParticleSystem)�� �Ҵ��ؾ� �մϴ�.

	// Ʈ���̼� ������Ʈ
	_tracerComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TracerComponent"));
	_tracerComponent->SetupAttachment(_barrelMesh);
	_tracerComponent->bAutoActivate = false;
	// �����Ϳ��� �ش� ������Ʈ�� Asset(NiagaraSystem)�� �Ҵ��ؾ� �մϴ�.
}

void ASentryTurret::BeginPlay()
{
	Super::BeginPlay();

	// �������Ʈ���� ������ _maxAmmo ���� �״�� ����Ͽ� _curAmmo �ʱ�ȭ
	_curAmmo = _maxAmmo;
	_curHp = _maxHp;
}

void ASentryTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASentryTurret::AIStartFire()
{
	// �̹� Ÿ�̸Ӱ� Ȱ��ȭ�Ǿ� �ְų�, fireInterval�� 0 ������ ��� ����
	if (_fireTimerHandle.IsValid() || _sentryData._fireInterval <= 0.0f)
	{
		return;
	}

	// SetTimer�� _sentryData._fireInterval ���ݸ��� Fire() ȣ���� �ݺ� ����
	GetWorldTimerManager().SetTimer(
		_fireTimerHandle,
		this,
		&ASentryTurret::Fire,
		_sentryData._fireInterval,
		true,   // �ݺ� ���
		0.0f    // ù ȣ�� ������
	);
}

void ASentryTurret::AIStopFire()
{
	// Ÿ�̸Ӱ� ��ȿ�ϴٸ� ����
	GetWorldTimerManager().ClearTimer(_fireTimerHandle);
}

void ASentryTurret::Fire()
{
	// (0) ź���� ������ �ı� ó��
	if (_curAmmo <= 0)
	{
		HandleOutOfAmmo();
		return;
	}

	// (1) ����Ʈ���̽��� �����ؼ� Hit ������ �����´�.
	FHitResult Hit = GetHitResult();

	// (2) Start �� End ������ ���� �������� ���� ����� ���� �׸���
	FVector Start = Hit.TraceStart;
	FVector End = Hit.bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd;

	DrawDebugLine(
		GetWorld(),
		Start,
		End,
		FColor::Red,
		false,   // Depth Test ���ʿ�
		1.0f,    // 1�� ���� ǥ��
		0,       // ��ũ�� ������ ����
		2.0f     // �� �β�
	);

	// ���� �÷��� ���
	PlayMuzzleFlash();

	// Ʈ���̼� ���
	PlayTracer(End);

	// (3) Hit�� �߻��� ��쿡�� ������ ��� �� ����� ����
	if (Hit.bBlockingHit)
	{
		// TraceStart�� ImpactPoint ������ �Ÿ�(cm) �� m ������ ��ȯ
		float DistanceCm = FVector::Dist(Hit.TraceStart, Hit.ImpactPoint);
		float DistanceMeters = DistanceCm / 100.0f;

		// CalculateDamage() ȣ��
		float DamageToApply = CalculateDamage(DistanceMeters);

		// ApplyDamage ȣ�� ����
		ApplyHitDamage(Hit, DamageToApply);
	}

	_curAmmo--;
	if (_curAmmo <= 0)
	{
		HandleOutOfAmmo();
		return;
	}
}

FHitResult ASentryTurret::GetHitResult() const
{
	FHitResult Hit;

	// �ѱ� ������ �����Ƿ� Actor ��ġ & ȸ������ �������� ������ ����
	FVector TraceStart = GetActorLocation();
	FVector TraceDirection = GetActorForwardVector();  // Actor�� ����
	FVector TraceEnd = TraceStart + (TraceDirection * _range);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel
	(
		Hit,
		TraceStart,
		TraceEnd,
		ECC_Visibility,
		Params
	);

	// ���� ����� ���ο����� ����
	Hit.TraceStart = TraceStart;
	Hit.TraceEnd = TraceEnd;
	return Hit;
}

float ASentryTurret::CalculateDamage(float distance) const
{
	// DistanceMeters ���� �Ÿ��� ������ ���� ���
	// (SentryData._falloff25 / _falloff50 / _falloff100 ���)
	if (distance <= 25.0f)
	{
		float Alpha = distance / 25.0f;
		float Falloff = FMath::Lerp(0.0f, _sentryData._falloff25, Alpha);
		return _sentryData._baseDamage * (1.0f - Falloff);
	}
	else if (distance <= 50.0f)
	{
		float Alpha = (distance - 25.0f) / 25.0f;
		float Falloff = FMath::Lerp(_sentryData._falloff25, _sentryData._falloff50, Alpha);
		return _sentryData._baseDamage * (1.0f - Falloff);
	}
	else if (distance <= 100.0f)
	{
		float Alpha = (distance - 50.0f) / 50.0f;
		float Falloff = FMath::Lerp(_sentryData._falloff50, _sentryData._falloff100, Alpha);
		return _sentryData._baseDamage * (1.0f - Falloff);
	}
	else
	{
		return _sentryData._baseDamage * (1.0f - _sentryData._falloff100);
	}
}

void ASentryTurret::ApplyHitDamage(const FHitResult& hit, float damage) 
{
	// (1) ������ ���� ��������
	AActor* HitActor = hit.GetActor();
	if (!HitActor)
	{
		return;
	}

	// (2) Unreal�� ������ �ý��� ��� ����
	UGameplayStatics::ApplyDamage(
		HitActor,
		damage,
		GetInstigatorController(), // �߻��ڸ� ����(������ nullptr)
		this,                      // �������� �� ��ü(=this)
		UDamageType::StaticClass() // �⺻ ������ Ÿ��
	);

	// ���Ѵٸ�, ���� ���Ͱ� �ı� ������ �������̽��� �����ߴٸ� ȣ���ϰų�,
	// �׽�Ʈ������ Destroy()�� �ٷ� �� ������ �˴ϴ�.
	// ����:
	// HitActor->Destroy();
}

void ASentryTurret::PlayMuzzleFlash()
{
	if (_muzzleFlashComponent)
	{
		_muzzleFlashComponent->Deactivate();
		_muzzleFlashComponent->Activate(true);
	}
}

void ASentryTurret::PlayTracer(const FVector& EndPoint)
{
	if (_tracerComponent)
	{
		_tracerComponent->SetVectorParameter(TEXT("BeamEnd"), EndPoint);
		_tracerComponent->Deactivate();
		_tracerComponent->Activate(true);
	}
}

float ASentryTurret::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// ������ ����
	_curHp -= DamageAmount;

	// ü�� 0 ���� �� �ı� ����
	if (_curHp <= 0.0f)
	{
		HandleOutOfAmmo();
	}

	return DamageAmount;
}

void ASentryTurret::HandleOutOfAmmo()
{
	AIStopFire();
	Destroy();
}


