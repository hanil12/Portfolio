// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_SuperSpeed.h"

UCAS_Task_SuperSpeed* UCAS_Task_SuperSpeed::Task_SuperSpeed(UGameplayAbility* OwningAbility, FName TaskName)
{
	UCAS_Task_SuperSpeed* Task = NewAbilityTask<UCAS_Task_SuperSpeed>(OwningAbility, TaskName);

	return Task;
}

void UCAS_Task_SuperSpeed::Activate()
{

	auto Handle = Ability->GetCurrentAbilitySpecHandle();
	auto ActorInfo = Ability->GetCurrentActorInfo();
	auto ActivationInfo = Ability->GetCurrentActivationInfo();

	OnAbilityEnd.Broadcast(Handle, ActorInfo, ActivationInfo, true, false);
}


