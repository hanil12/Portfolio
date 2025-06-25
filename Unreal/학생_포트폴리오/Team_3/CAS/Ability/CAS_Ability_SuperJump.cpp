// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_SuperJump.h"
#include "Ability_Task/CAS_Task_SuperJump.h"

UCAS_Ability_SuperJump::UCAS_Ability_SuperJump()
{
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("State.TakeDamage"));
}

bool UCAS_Ability_SuperJump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

}

void UCAS_Ability_SuperJump::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	auto Task = UCAS_Task_SuperJump::Task_SuperJump(this, "SuperJump");
	if (Task->IsValidLowLevel()) {
		Task->OnAbilityEnd.AddUObject(this, &ThisClass::EndAbility);
		Task->ReadyForActivation();

		auto owner = Cast<ACAS_Character>(GetGameplayTaskAvatar(Task));

		JumpTarget(owner, 1);
	}

}

void UCAS_Ability_SuperJump::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

FActiveGameplayEffectHandle UCAS_Ability_SuperJump::ApplyGamePlayEffect(ACAS_Character* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel, const FGameplayEffectContextHandle& EffectContext, UAbilitySystemComponent* AbilitySystemComponent)
{
	UAbilitySystemComponent* TargetAbilitySystemComp = Target->GetAbilitySystemComponent();

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContext);
	if (SpecHandle.IsValid())
	{
		//SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Effect.Move.SuperSpeed")), 2.0f);
		FActiveGameplayEffectHandle Handle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetAbilitySystemComp);

		return Handle;
	}

	return FActiveGameplayEffectHandle();
}

void UCAS_Ability_SuperJump::JumpTarget(ACAS_Character* Target, int32 TaskLevel)
{
	if (!TagEffectClassJump)
		return;

	auto PlayerState = Cast<ACAS_PlayerState>(GetOwningActorFromActorInfo());
	UAbilitySystemComponent* AbilitySystemComp;
	if (PlayerState->IsValidLowLevel()) {
		AbilitySystemComp = PlayerState->GetAbilitySystemComponent();
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComp->MakeEffectContext();
		EffectContextHandle.AddInstigator(PlayerState, nullptr);

		if (AbilitySystemComp->GetActiveGameplayEffect(ActiveEffectHandle) == nullptr)
		{
			FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(TagEffectClassJump, GetAbilityLevel());
			ActiveEffectHandle = ApplyGamePlayEffect(Target, TagEffectClassJump, TaskLevel, EffectContextHandle, AbilitySystemComp);
		}
		else
		{
			AbilitySystemComp->RemoveActiveGameplayEffect(ActiveEffectHandle);
			ActiveEffectHandle = FActiveGameplayEffectHandle();
		}
	}
	else {
		auto CharacterState = Cast<ACAS_Character>(GetOwningActorFromActorInfo());
		if (CharacterState->IsValidLowLevel()) {
			AbilitySystemComp = CharacterState->GetAbilitySystemComponent();

			FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComp->MakeEffectContext();
			EffectContextHandle.AddInstigator(CharacterState, nullptr);

			if (AbilitySystemComp->GetActiveGameplayEffect(ActiveEffectHandle) == nullptr)
			{
				FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(TagEffectClassJump, GetAbilityLevel());
				ActiveEffectHandle = ApplyGamePlayEffect(Target, TagEffectClassJump, TaskLevel, EffectContextHandle, AbilitySystemComp);
			}
			else
			{
				AbilitySystemComp->RemoveActiveGameplayEffect(ActiveEffectHandle);
				ActiveEffectHandle = FActiveGameplayEffectHandle();
			}
		}
	}



}
