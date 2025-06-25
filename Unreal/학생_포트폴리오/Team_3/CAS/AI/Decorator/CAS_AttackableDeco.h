// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CAS_AttackableDeco.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_AttackableDeco : public UBTDecorator
{
	GENERATED_BODY()
public:
	UCAS_AttackableDeco();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector TargetKey;
};
