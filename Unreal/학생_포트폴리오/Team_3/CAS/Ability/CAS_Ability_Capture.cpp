// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_Capture.h"
#include "Ability_Task/CAS_Task_Capture.h"
UCAS_Ability_Capture::UCAS_Ability_Capture()
{
}

bool UCAS_Ability_Capture::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UCAS_Ability_Capture::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	auto Task = UCAS_Task_Capture::Task_Capture(this, "Capture");
	if (Task->IsValidLowLevel()) {
		Task->OnAbilityEnd.AddUObject(this, &ThisClass::EndAbility);
		Task->ReadyForActivation();
	}
}

void UCAS_Ability_Capture::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}
