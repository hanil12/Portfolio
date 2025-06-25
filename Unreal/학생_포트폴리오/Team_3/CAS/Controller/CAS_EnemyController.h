// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAS_EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_EnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	ACAS_EnemyController();

	virtual void OnPossess(APawn* pawn) override;
	virtual void OnUnPossess() override;

	UFUNCTION()
	void RandMove();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBlackboardData* BlackboardData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* BehaviorTree;

private:
	UPROPERTY()
	FTimerHandle TimerHandle;

};
