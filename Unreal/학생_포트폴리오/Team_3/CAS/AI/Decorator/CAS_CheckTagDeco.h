// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_CheckGameplayTagsOnActor.h"
#include "CAS_CheckTagDeco.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_CheckTagDeco : public UBTDecorator_CheckGameplayTagsOnActor
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "GameplayTag")
    FGameplayTag TagToCheck;

protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
