// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_Capture.h"
#include "Character/CAS_Player.h"
#include "Character/CAS_Hat.h"
UCAS_Task_Capture* UCAS_Task_Capture::Task_Capture(UGameplayAbility* OwningAbility, FName TaskName)
{
	UCAS_Task_Capture* Task = NewAbilityTask<UCAS_Task_Capture>(OwningAbility, TaskName);

	return Task;
}

void UCAS_Task_Capture::Activate()
{
	auto player = Cast<ACAS_Player>((GetAvatarActor()));
	if (player->IsValidLowLevel()) {

		ACAS_Hat* Hat =  player->GetHat();

		auto dir = player->GetActorForwardVector();

		Hat->Throw(dir);
		Hat->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);


		auto Handle = Ability->GetCurrentAbilitySpecHandle();
		auto ActorInfo = Ability->GetCurrentActorInfo();
		auto ActivationInfo = Ability->GetCurrentActivationInfo();

		OnAbilityEnd.Broadcast(Handle, ActorInfo, ActivationInfo, true, false);

	}
}
