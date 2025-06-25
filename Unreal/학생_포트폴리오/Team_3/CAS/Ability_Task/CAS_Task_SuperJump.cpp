// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_SuperJump.h"
#include "Character/CAS_EnemyCapt.h"

UCAS_Task_SuperJump* UCAS_Task_SuperJump::Task_SuperJump(UGameplayAbility* OwningAbility, FName TaskName)
{
	UCAS_Task_SuperJump* Task = NewAbilityTask<UCAS_Task_SuperJump>(OwningAbility, TaskName);

	return Task;
}

void UCAS_Task_SuperJump::Activate()
{

	auto Handle = Ability->GetCurrentAbilitySpecHandle();
	auto ActorInfo = Ability->GetCurrentActorInfo();
	auto ActivationInfo = Ability->GetCurrentActivationInfo();

	OnAbilityEnd.Broadcast(Handle, ActorInfo, ActivationInfo, true, false);
}
