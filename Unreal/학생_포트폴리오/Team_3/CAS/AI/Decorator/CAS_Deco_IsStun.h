// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CAS_Deco_IsStun.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_Deco_IsStun : public UBTDecorator
{
	GENERATED_BODY()
public:
	UCAS_Deco_IsStun();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
