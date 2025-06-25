// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CAS_AbilitySystemComponent.h"
#include "GAS/CAS_GameplayAbility.h"
void UCAS_AbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<class UGameplayAbility>>& Abilities)
{
	for (auto& AbilityClass : Abilities) {
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		FGameplayAbilitySpecHandle specHandle = GiveAbility(AbilitySpec); // 스킬을 배우는 단계 
		//게임플레이어빌리티스펙핸들 을 반환
		// 핸들 : 식별을 위해 달아놓은 포인터
		SpecHandles.Add(specHandle);
	}
}

void UCAS_AbilitySystemComponent::RemoveAbility(const FGameplayTag& Tag)
{
	TArray<FGameplayAbilitySpecHandle> SpecsToRemove;

	for (const auto& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability && Spec.Ability->AbilityTags.HasTag(Tag))
		{
			SpecsToRemove.Add(Spec.Handle);
		}
	}

	for (const FGameplayAbilitySpecHandle& Handle : SpecsToRemove)
	{
		ClearAbility(Handle);
	}
}

void UCAS_AbilitySystemComponent::ActivateAbility(const FGameplayTag& Tag)
{
	if (FGameplayAbilitySpec* Spec = FindAbilitySpecByTag(Tag))
	{
		TryActivateAbility(Spec->Handle);
	}
}

void UCAS_AbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UCAS_GameplayAbility*>& Abilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	// Iterate the list of all ability specs
	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		// Iterate all instances on this ability spec
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			Abilities.Add(Cast<UCAS_GameplayAbility>(ActiveAbility));
		}
	}
}



FGameplayAbilitySpec* UCAS_AbilitySystemComponent::FindAbilitySpecByTag(const FGameplayTag& Tag)
{
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.Ability && Spec.Ability->AbilityTags.HasTag(Tag))
		{
			return &Spec;
		}
	}
	return nullptr;
}
