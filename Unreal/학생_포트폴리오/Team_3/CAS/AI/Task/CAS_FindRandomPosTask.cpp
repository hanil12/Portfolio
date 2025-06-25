// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/CAS_FindRandomPosTask.h"

#include "AIController.h"
#include "NavigationSystem.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UCAS_FindRandomPosTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto curPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (!curPawn->IsValidLowLevel())
		return EBTNodeResult::Type();

	FVector pos = curPawn->GetActorLocation();

	auto nav = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (!nav->IsValidLowLevel())
		return EBTNodeResult::Type::Failed;

	FNavLocation randomLocation;
	if (nav->GetRandomPointInNavigableRadius(pos, 1300.0f, randomLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("Destination")), randomLocation.Location);

		return EBTNodeResult::Type::Succeeded;
	}

	return EBTNodeResult::Type::Failed;
}
