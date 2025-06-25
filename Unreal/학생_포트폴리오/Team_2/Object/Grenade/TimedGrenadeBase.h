// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Throwable.h"
#include "TimedGrenadeBase.generated.h"

/**
 * 
 */
UCLASS()
class SGAPROJECTMAIN_API ATimedGrenadeBase : public AThrowable
{
	GENERATED_BODY()
	
public:
	virtual void StartCookingGrenade();
	virtual void UpdateCookingGrenade();
	virtual void Throw(FVector direction) override;
	virtual void ExplodeGrenade();

	bool IsFuseTimeRemaining();
	float GetRemainingFuseTime();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game/Grenade")
	float _totalFuseTime = 5.0f; // ������ ���� �ɸ��� �ð�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game/Grenade")
	float _cookedTime = 0.0f; // ��ŷ �ð�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game/Grenade")
	float _explosionRadius = 300.0f; // ���� �ݰ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game/Grenade")
	float _explosionDamage = 100.0f; // ���� ������

	bool _isExploded = false;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	UParticleSystem* _explosionEffect; // ���� ����Ʈ

	FTimerHandle _explosionTimerHandle;
};
