// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/CAS_FindPlayerService.h"

#include "Controller/CAS_EnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CAS_Character.h"
#include "Character/CAS_Player.h"
#include "Engine/OverlapResult.h"

UCAS_FindPlayerService::UCAS_FindPlayerService()
{
	NodeName = TEXT("FindPlayerService");
	Interval = 0.5f;
}

void UCAS_FindPlayerService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* curPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (!curPawn->IsValidLowLevel()){
		return;
	}

	FVector pos = curPawn->GetActorLocation();
	
	TArray<FOverlapResult> overlapResults;
	FCollisionQueryParams qParams(NAME_None, false, curPawn);

	bool result = GetWorld()->OverlapMultiByChannel(
		overlapResults,
		pos,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(Radius),
		qParams
	);

	DrawDebugSphere(GetWorld(), pos, Radius, 30, FColor::Green, false, 0.2f);

	if (!result)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetKey.SelectedKeyName, nullptr);
		return;
	}
	else
	{
		for (auto& overlapResult : overlapResults)
		{
			auto player = Cast<ACAS_Player>(overlapResult.GetActor());
			
			if (player->IsValidLowLevel())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetKey.SelectedKeyName, player);
				DrawDebugSphere(GetWorld(), pos, Radius, 30, FColor::Red, false, 0.2f);
				return;
			}
		}

		OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetKey.SelectedKeyName, nullptr);
		return;
	}


}
