// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CAS_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)			\
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)	\
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)				\
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)				\
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)				

DECLARE_MULTICAST_DELEGATE_OneParam(FHpChangedEvent, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FSpeedChangedEvent, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FJumpForceChangedEvent, int32);
DECLARE_MULTICAST_DELEGATE(FDeadEvent);

UCLASS()
class CAS_API UCAS_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UCAS_AttributeSet();
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

public:
	ATTRIBUTE_ACCESSORS(ThisClass, Health);
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHealth);
	ATTRIBUTE_ACCESSORS(ThisClass, WalkSpeed);
	ATTRIBUTE_ACCESSORS(ThisClass, JumpForce);
	
	FHpChangedEvent HpChanged;
	FSpeedChangedEvent SpeedChanged;
	FJumpForceChangedEvent JumpForceChanged;
	FDeadEvent DeadEvent;


private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxHealth;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData WalkSpeed;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData JumpForce;
};