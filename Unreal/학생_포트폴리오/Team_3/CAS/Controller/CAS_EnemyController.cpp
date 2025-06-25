// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CAS_EnemyController.h"

ACAS_EnemyController::ACAS_EnemyController()
{
}

void ACAS_EnemyController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	UBlackboardComponent* BlackBoardComp = Blackboard;
	UseBlackboard(BlackboardData, BlackBoardComp);
	RunBehaviorTree(BehaviorTree);
}

void ACAS_EnemyController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAS_EnemyController::RandMove()
{
}
