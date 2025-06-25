// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CAS_EnemyCapt.h"
#include "CAS_EnemyCapt_SuperSpeed.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_EnemyCapt_SuperSpeed : public ACAS_EnemyCapt
{
	GENERATED_BODY()

public:

	ACAS_EnemyCapt_SuperSpeed();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//void ActivateAbility(const FInputActionValue& Value);

	virtual void BeginPlay() override;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//UInputAction* AbilityAction;
};
