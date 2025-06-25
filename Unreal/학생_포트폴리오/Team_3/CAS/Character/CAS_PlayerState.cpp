// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CAS_PlayerState.h"
#include "GAS/CAS_AttributeSet.h"
#include "GAS/CAS_AbilitySystemComponent.h"
ACAS_PlayerState::ACAS_PlayerState()
{
    AbilitySystemComponent = CreateDefaultSubobject<UCAS_AbilitySystemComponent>("AbilitySystemComponent");
    AttributeSet = CreateDefaultSubobject<UCAS_AttributeSet>("PlayerAttributeSet");
}

UAbilitySystemComponent* ACAS_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCAS_AttributeSet* ACAS_PlayerState::GetAttributeSet() const
{
	return AttributeSet;
}
