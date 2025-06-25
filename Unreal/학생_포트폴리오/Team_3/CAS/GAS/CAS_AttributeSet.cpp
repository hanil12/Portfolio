// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CAS_AttributeSet.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffectTypes.h"

UCAS_AttributeSet::UCAS_AttributeSet()
{
	InitHealth(3.0f);
	InitMaxHealth(6.0f);
	InitWalkSpeed(500.0f);
	InitJumpForce(700.0f);

}

void UCAS_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
}

void UCAS_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		int32 Hp = FMath::RoundToInt(GetHealth());
		if (Hp <= 0)
		{
			DeadEvent.Broadcast();
		}
		
		HpChanged.Broadcast(Hp);
		
	}

	if (Data.EvaluatedData.Attribute == GetWalkSpeedAttribute())
	{
		int32 speed = GetWalkSpeed();
		SpeedChanged.Broadcast(speed);
	}

	if (Data.EvaluatedData.Attribute == GetJumpForceAttribute())
	{
		int32 force = GetJumpForce();
		JumpForceChanged.Broadcast(force);
	}
}
