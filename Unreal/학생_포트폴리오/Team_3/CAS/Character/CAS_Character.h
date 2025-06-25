// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GAS/CAS_AttributeSet.h"
#include "GAS/CAS_GamePlayTag.h"
#include "AbilitySystemInterface.h"
#include "GAS/CAS_AbilitySystemComponent.h"
#include "CAS_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(Abstract)
class CAS_API ACAS_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACAS_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void DeadEvent();
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const;
	virtual UCAS_AttributeSet* GetAttributeSet() const	{PURE_VIRTUAL(ACAS_Player::GetAttributeSet, return nullptr;);}

	virtual void AddDefaultAbilites();
	virtual void InitAbilitySystemComponent(AController* controller);
	void SetHp(int32 value);
	void SetWalkSpeed(int32 value);

	void SetJumpForce(float value);
	void OnJumpForceChanged(const FOnAttributeChangeData& Data);

	virtual void ActivateAbility(const FGameplayTag tag);
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UCAS_GameplayAbility*>& Abilities);
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	bool CanActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation = true);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	TObjectPtr<class UCAS_AbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	TObjectPtr<class UCAS_AttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HpBar")
	class UWidgetComponent* HpBarWidgetComponent;
	UPROPERTY(EditAnywhere, Category = "HpBar")
	TSubclassOf<class UUserWidget> HpBarWidgetClass;
	UPROPERTY(EditAnywhere, Category = "HpBar")
	int32 HpCount = 3;
};

