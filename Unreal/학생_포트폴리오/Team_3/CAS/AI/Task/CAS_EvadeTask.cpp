// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_EvadeTask.h"
#include "Controller/CAS_EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CAS_Character.h"
#include "Character/CAS_Player.h"
#include "Kismet/KismetMathLibrary.h"

UCAS_EvadeTask::UCAS_EvadeTask()
{
}

EBTNodeResult::Type UCAS_EvadeTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    auto curPawn = Cast<ACAS_Character>(OwnerComp.GetAIOwner()->GetPawn());
    auto player = Cast<ACAS_Player>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(Key.SelectedKeyName));

    if (!curPawn->IsValidLowLevel() || !player->IsValidLowLevel()) {
        return EBTNodeResult::Failed;
    }

    auto quat = UKismetMathLibrary::FindLookAtRotation(curPawn->GetActorLocation(), player->GetActorLocation());
    curPawn->SetActorRotation(quat);
    curPawn->ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.Move.Evade"));
    return EBTNodeResult::Succeeded;
}
