// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_Dead.h"
#include "GAS/CAS_GamePlayTag.h"
UCAS_Task_Dead* UCAS_Task_Dead::Task_Dead(UGameplayAbility* OwningAbility, FName TaskName, UAnimMontage* MontageToPlay, float Rate)
{
	if (!MontageToPlay) {
		return nullptr;
	}
	UCAS_Task_Dead* Task = NewAbilityTask<UCAS_Task_Dead>(OwningAbility, TaskName);

	Task->Montage = MontageToPlay;
	Task->PlayRate = Rate;

	return Task;
}

void UCAS_Task_Dead::Activate()
{
	Super::Activate();

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

void UCAS_Task_Dead::PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	auto Character = Cast<ACAS_Character>((GetAvatarActor()));

	Character->SetActorHiddenInGame(true);
	Character->SetActorEnableCollision(false);

	auto Handle = Ability->GetCurrentAbilitySpecHandle();
	auto ActorInfo = Ability->GetCurrentActorInfo();
	auto ActivationInfo = Ability->GetCurrentActivationInfo();

	auto ASC = Character->GetAbilitySystemComponent();
	ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead"));

	Character->Controller->UnPossess();
	OnAbilityEnd.Broadcast(Handle, ActorInfo, ActivationInfo, true, false);

}
