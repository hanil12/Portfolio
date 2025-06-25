// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_AbilityTask.h"
#include "CAS_Task_Dead.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_Task_Dead : public UCAS_AbilityTask
{
	GENERATED_BODY()
public:
	static UCAS_Task_Dead* Task_Dead(UGameplayAbility* OwningAbility, FName TaskName, UAnimMontage* MontageToPlay, float Rate = 1.0f);
protected:
	virtual void Activate() override;
	virtual void PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload) override;
};
