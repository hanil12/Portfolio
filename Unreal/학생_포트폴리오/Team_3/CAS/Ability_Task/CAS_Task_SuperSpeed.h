// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_AbilityTask.h"
#include "CAS_Task_SuperSpeed.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_Task_SuperSpeed : public UCAS_AbilityTask
{
	GENERATED_BODY()

public:
	static UCAS_Task_SuperSpeed* Task_SuperSpeed(UGameplayAbility* OwningAbility, FName TaskName);

	FAbilityEndEvent OnAbilityEnd;

protected:
	virtual void Activate() override;

};
