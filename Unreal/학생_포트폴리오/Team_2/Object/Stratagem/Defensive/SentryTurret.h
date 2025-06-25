// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SentryTurret.generated.h"

USTRUCT(BlueprintType)
struct FSentryData
{
	GENERATED_BODY()

	FSentryData()
		: _baseDamage(20.0f)
		, _falloff25(0.1f)
		, _falloff50(0.2f)
		, _falloff100(0.5f)
		, _fireInterval(0.1f)
	{}

	// �� �ߴ� �⺻ ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game/Stratagem/Sentry")
	float _baseDamage;

	// 25m �Ÿ������� �ִ� ������ ���� ���� (0~1)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game/Stratagem/Sentry")
	float _falloff25;

	// 50m �Ÿ������� �ִ� ������ ���� ���� (0~1)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game/Stratagem/Sentry")
	float _falloff50;

	// 100m �̻� �Ÿ������� �ִ� ������ ���� ���� (0~1)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game/Stratagem/Sentry")
	float _falloff100;

	// �߻� ����(�� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game/Stratagem/Sentry")
	float _fireInterval;
};

UCLASS()
class SGAPROJECTMAIN_API ASentryTurret : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASentryTurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ��Ʈ���� �� �������� �ڵ� ����� �����մϴ�.
	UFUNCTION(BlueprintCallable, Category = "Game/Stratagem/Sentry")
	void AIStartFire();

	// ��Ʈ���� �ڵ� ����� �ߴ��մϴ�.
	UFUNCTION(BlueprintCallable, Category = "Game/Stratagem/Sentry")
	void AIStopFire();

	// ������ �� ���� �߻��մϴ�. (AIStartFire�� ���� Timer�� ȣ��)
	void Fire();

	// ����Ʈ���̽� ���� �� �Ÿ� ��� �� HitResult�� ��ȯ�մϴ�.
	FHitResult GetHitResult() const;

	// �Ÿ�(dist, ����: m)�� ���� �������� ����Ͽ� ��ȯ�մϴ�.
	float CalculateDamage(float distance) const;

	// ���� ���Ϳ��� ApplyDamage�� ȣ�� �Լ� 
	void ApplyHitDamage(const FHitResult& hit, float damage);

	// �ܺο��� �������� ���� �� ȣ��Ǵ� �������̵� �Լ�
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void HandleOutOfAmmo();

	// ���� �÷��� ����Ʈ�� ����մϴ�.
	void PlayMuzzleFlash();

	// Ʈ���̼� ����Ʈ�� ����մϴ�.
	void PlayTracer(const FVector& EndPoint);


protected:
	// �ͷ� ����(�ڽ� ��)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game/Stratagem/Sentry")
	UStaticMeshComponent* _bodyMesh;

	// �ͷ� �ѱ�(����)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game/Stratagem/Sentry")
	UStaticMeshComponent* _barrelMesh;

	// �ִ� �����Ÿ� (cm ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game/Stratagem/Sentry")
	float _range = 10000.0f;

	// ���� �÷��ÿ� ParticleSystemComponent�� �̸� ���� �� Activate/Deactivate�� ���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game/Stratagem/Sentry")
	UParticleSystemComponent* _muzzleFlashComponent;

	// Ʈ���̼��� NiagaraComponent�� �̸� ���� �� BeamEnd �Ķ���� ���� �� Activate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game/Stratagem/Sentry")
	class UNiagaraComponent* _tracerComponent;

	// �ִ� ź�� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game/Stratagem/Sentry")
	int32 _maxAmmo;

	// ���� ���� ź�� ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game/Stratagem/Sentry")
	int32 _curAmmo;

	// ��Ʈ�� ��� ������ (������, �Ÿ��� ����, �߻� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game/Stratagem/Sentry")
	FSentryData _sentryData;

	// �ڵ� ��� Ÿ�̸� �ڵ�
	FTimerHandle _fireTimerHandle;

	// �ִ� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game/Stratagem/Sentry")
	float _maxHp;

	// ���� ü��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game/Stratagem/Sentry")
	float _curHp;
};
