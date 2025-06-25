// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_SuperSpeed.h"
#include "Ability_Task/CAS_Task_SuperSpeed.h"
#include "Character/CAS_Character.h"

UCAS_Ability_SuperSpeed::UCAS_Ability_SuperSpeed()
{
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("State.TakeDamage"));

}

bool UCAS_Ability_SuperSpeed::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);

}

void UCAS_Ability_SuperSpeed::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	auto Task = UCAS_Task_SuperSpeed::Task_SuperSpeed(this, "SuperSpeed");
	if (Task->IsValidLowLevel()) {
		Task->OnAbilityEnd.AddUObject(this, &ThisClass::EndAbility);
		Task->ReadyForActivation();

		auto owner = Cast<ACAS_Character>(GetGameplayTaskAvatar(Task));

		ReceiveTarget(owner, 1);

	}
}

void UCAS_Ability_SuperSpeed::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

FActiveGameplayEffectHandle  UCAS_Ability_SuperSpeed::ApplyGamePlayEffect(ACAS_Character* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel, const FGameplayEffectContextHandle& EffectContext, UAbilitySystemComponent* AbilitySystemComponent)
{
	UAbilitySystemComponent* TargetAbilitySystemComp = Target->GetAbilitySystemComponent();

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContext);
	if (SpecHandle.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Effect.Move.SuperSpeed")), 2.0f);
		FActiveGameplayEffectHandle Handle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetAbilitySystemComp);

		return Handle;
	}

	return FActiveGameplayEffectHandle();
}

void UCAS_Ability_SuperSpeed::ReceiveTarget(ACAS_Character* Target, int32 TaskLevel)
{
	if (!TagEffectClass || !TagEffectClassEnemy || !TagEffectClassPlayer) {
		return;
	}

	auto PlayerState = Cast<ACAS_PlayerState>(GetOwningActorFromActorInfo());
	UAbilitySystemComponent* AbilitySystemComp;
	if (PlayerState->IsValidLowLevel()) {
		AbilitySystemComp = PlayerState->GetAbilitySystemComponent();
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComp->MakeEffectContext();
		EffectContextHandle.AddInstigator(PlayerState, nullptr);

		// Delay 타이머 설정
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate;

		ApplyGamePlayEffect(Target, TagEffectClass, TaskLevel, EffectContextHandle, AbilitySystemComp);
		FActiveGameplayEffectHandle Handle = ApplyGamePlayEffect(Target, TagEffectClassEnemy, TaskLevel, EffectContextHandle, AbilitySystemComp);

		TimerDelegate.BindLambda([this, Target, TaskLevel, EffectContextHandle, AbilitySystemComp, Handle]()
		{
			ApplyGamePlayEffect(Target, TagEffectClassPlayer, TaskLevel, EffectContextHandle, AbilitySystemComp);
			AbilitySystemComp->RemoveActiveGameplayEffect(Handle);
		});

		Target->GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 5.0f, false);


	}
	else {
		auto CharacterState = Cast<ACAS_Character>(GetOwningActorFromActorInfo());
		if (CharacterState->IsValidLowLevel()) {
			AbilitySystemComp = CharacterState->GetAbilitySystemComponent();
			FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComp->MakeEffectContext();
			EffectContextHandle.AddInstigator(CharacterState, nullptr);

			ApplyGamePlayEffect(Target, TagEffectClass, TaskLevel, EffectContextHandle, AbilitySystemComp);
			ApplyGamePlayEffect(Target, TagEffectClassEnemy, TaskLevel, EffectContextHandle, AbilitySystemComp);
		}
	}

}
