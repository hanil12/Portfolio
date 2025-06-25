// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GAS/CAS_GameplayAbility.h"
#include "CAS_AbilityTask.generated.h"

DECLARE_MULTICAST_DELEGATE_FiveParams(FAbilityEndEvent, FGameplayAbilitySpecHandle, const FGameplayAbilityActorInfo*, FGameplayAbilityActivationInfo, bool, bool);

UCLASS()
class CAS_API UCAS_AbilityTask : public UAbilityTask
{
	GENERATED_BODY()
	
public:

	FAbilityEndEvent OnAbilityEnd;
protected:
	UFUNCTION()
	virtual void PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> Montage;
	UPROPERTY()
	float PlayRate = 1.0f;
};


