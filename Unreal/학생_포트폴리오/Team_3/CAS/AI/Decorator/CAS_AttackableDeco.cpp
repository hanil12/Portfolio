// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/CAS_AttackableDeco.h"
#include "Character/CAS_Player.h"
#include "Controller/CAS_EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
UCAS_AttackableDeco::UCAS_AttackableDeco()
{
    NodeName = TEXT("Attackable");
}

bool UCAS_AttackableDeco::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

    APawn* curPawn = OwnerComp.GetAIOwner()->GetPawn();

    if (ACAS_Player* player = Cast<ACAS_Player>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName))) {

        auto distance = player->GetDistanceTo(curPawn);

        return distance <= 100.0f;
    }
    
    return false;
}
