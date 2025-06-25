// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CAS_FindPlayerService.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_FindPlayerService : public UBTService
{
	GENERATED_BODY()
public:

	UCAS_FindPlayerService();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Radius = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector TargetKey;
};
