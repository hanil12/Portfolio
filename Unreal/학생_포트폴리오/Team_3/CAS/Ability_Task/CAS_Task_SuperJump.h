// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/CAS_AbilityTask.h"
#include "CAS_Task_SuperJump.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_Task_SuperJump : public UCAS_AbilityTask
{
	GENERATED_BODY()

public:
	static UCAS_Task_SuperJump* Task_SuperJump(UGameplayAbility* OwningAbility, FName TaskName);

	FAbilityEndEvent OnAbilityEnd;

protected:
	virtual void Activate() override;
};
