// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/CAS_Deco_IsStun.h"
#include "Character/CAS_EnemyCapt.h"
#include "AIController.h"

UCAS_Deco_IsStun::UCAS_Deco_IsStun()
{
}

bool UCAS_Deco_IsStun::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	auto curPawn = Cast<ACAS_EnemyCapt>(OwnerComp.GetAIOwner()->GetPawn());

	if (curPawn->IsStunned())
		return true;

	return false;
}
