// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "CAS_AbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AddCharacterAbilities(const TArray<TSubclassOf<class UGameplayAbility>>& Abilities);
	void RemoveAbility(const FGameplayTag& Tag);
	void ActivateAbility(const FGameplayTag& Tag);
	void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<class UCAS_GameplayAbility*>& Abilities);

private:
	TArray<FGameplayAbilitySpecHandle> SpecHandles;
	FGameplayAbilitySpec* FindAbilitySpecByTag(const FGameplayTag& Tag);
};
