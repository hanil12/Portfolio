// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Decorator/CAS_EvadeDeco.h"
#include "AIController.h"
#include "Character/EnemyCapt/CAS_EnemyCapt_Evade.h"

UCAS_EvadeDeco::UCAS_EvadeDeco()
{
}

bool UCAS_EvadeDeco::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto curPawn = Cast<ACAS_EnemyCapt_Evade>(OwnerComp.GetAIOwner()->GetPawn());

	if (curPawn->IsValidLowLevel())
		return true;

	return false;
}
