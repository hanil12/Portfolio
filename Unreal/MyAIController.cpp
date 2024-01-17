// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "NavigationSystem.h"
#include "BluePrint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AMyAIController::AMyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/AI/BT_MyBT.BT_MyBT'"));

	if (BT.Succeeded())
	{
		_bt = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB(TEXT("BlackboardData'/Game/AI/BB_MyBBData.BB_MyBBData'"));

	if (BB.Succeeded())
	{
		_bb = BB.Object;
	}
}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//GetWorld()->GetTimerManager().SetTimer(_timerHandle, this, &AMyAIController::RandMove, 3.0f, true);

	if (UseBlackboard(_bb, Blackboard))
	{
		if (RunBehaviorTree(_bt))
		{
			UE_LOG(LogTemp, Warning, TEXT("Behavior Tree Succeeded"));
		}
	}
}

void AMyAIController::OnUnPossess()
{
	Super::OnUnPossess();

	//GetWorld()->GetTimerManager().ClearTimer(_timerHandle);
}

void AMyAIController::RandMove()
{
	auto currentPawn = GetPawn();

	if (!IsValid(currentPawn))
		return;

	UNavigationSystemV1* naviSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if(naviSystem == nullptr)
		return;

	FNavLocation randLocation;

	naviSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, randLocation);
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, randLocation);

}
