// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CAS_Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CAS_EnemyCapt.generated.h"


/**
 * 
 */
UCLASS()
class CAS_API ACAS_EnemyCapt : public ACAS_Character
{
	GENERATED_BODY()
	

public:
	ACAS_EnemyCapt();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	virtual class UCAS_AttributeSet* GetAttributeSet() const override { return AttributeSet; }

	void BeCaptured(class ACAS_Hat* hat);

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	ACAS_Hat* GetHat() { return _hat; }
	void ClearHat() { _hat = nullptr; }
	bool IsCaptured() { return _isCaptured; }
	void ActivateCapture(bool value) { _isCaptured = value; }
	void DeCaptureAbility(const FInputActionValue& Value);
	void Stunning(float DeltaTime);
	void BeStunned() { _isStun = true; };
	bool IsStunned() { return _isStun; }

	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;
	virtual void AddDefaultAbilites() override;
	void AddPlayerAbility(AActor* actor);
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DeCaptureAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayAbility> EnemyAbility;

	UPROPERTY(VisibleAnywhere, Category = "Hat")
	class ACAS_Hat* _hat;

	bool _isCaptured = false;

	bool _isStun = false;
	float _stunTime = 0.0f;
	float _maxStunTime = 5.0f;


};
