// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CAS_AttackTask.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_AttackTask : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UCAS_AttackTask();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Key;
};
