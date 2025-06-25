// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "CAS_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ACAS_PlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	class UCAS_AttributeSet* GetAttributeSet() const;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	TObjectPtr<class UCAS_AbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	class UCAS_AttributeSet* AttributeSet;
};