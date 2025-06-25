// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_Evade.h"
#include "Character/CAS_Character.h"


UCAS_Task_Evade* UCAS_Task_Evade::Task_Evade(UGameplayAbility* OwningAbility, FName TaskName, UAnimMontage* MontageToPlay, float Rate)
{
	if (!MontageToPlay) {
		return nullptr;
	}
	UCAS_Task_Evade* Task = NewAbilityTask<UCAS_Task_Evade>(OwningAbility, TaskName);

	Task->Montage = MontageToPlay;
	Task->PlayRate = Rate;

	return Task;
}

void UCAS_Task_Evade::Activate()
{
	if (!Montage) {
		return;
	}
	auto Character = Cast<ACAS_Character>((GetAvatarActor()));
	if (Character->IsValidLowLevel()) {

		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

		if (!AnimInstance)
		{
			return;
		}

		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ThisClass::PlayAnimNotify);
		AnimInstance->Montage_Play(Montage, PlayRate);
	}
}

void UCAS_Task_Evade::PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{


	auto Character = Cast<ACAS_Character>((GetAvatarActor()));
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &ThisClass::PlayAnimNotify);

	auto Handle = Ability->GetCurrentAbilitySpecHandle();
	auto ActorInfo = Ability->GetCurrentActorInfo();
	auto ActivationInfo = Ability->GetCurrentActivationInfo();

	OnAbilityEnd.Broadcast(Handle, ActorInfo, ActivationInfo, true, false);
}
