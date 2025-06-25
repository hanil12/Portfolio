// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_FindPlayerTask.h"
#include "Controller/CAS_EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CAS_Character.h"
#include "Character/CAS_Player.h"
#include "NavigationSystem.h"
UCAS_FindPlayerTask::UCAS_FindPlayerTask()
{
    NodeName = TEXT("FindPlayerTask");
}

EBTNodeResult::Type UCAS_FindPlayerTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    auto curPawn = Cast<ACAS_Character>(OwnerComp.GetAIOwner()->GetPawn());
    auto player = Cast<ACAS_Player>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(Key.SelectedKeyName));

    if (curPawn->IsValidLowLevel() || player->IsValidLowLevel()) {
        return EBTNodeResult::Failed;
    }
	//UNavigationSystemV1* NaviSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    //
	//FNavLocation Next;
    //if (NaviSystem->GetRandomPointInNavigableRadius(curPawn->GetActorLocation(), Radius, OUT Next)) {
    //    OwnerComp.GetBlackboardComponent()->SetValueAsVector(Key.SelectedKeyName, Next);
    //    return EBTNodeResult::Succeeded;
    //}
    return EBTNodeResult::Failed;
}

