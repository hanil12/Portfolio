// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/CAS_CheckTagDeco.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"

bool UCAS_CheckTagDeco::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

    AActor* curActor = OwnerComp.GetAIOwner()->GetPawn();
    if (!curActor->IsValidLowLevel())
        return false;

    UAbilitySystemComponent* ASC = curActor->FindComponentByClass<UAbilitySystemComponent>();
    if (!ASC) 
        return false;

    return ASC->HasMatchingGameplayTag(TagToCheck);
}
