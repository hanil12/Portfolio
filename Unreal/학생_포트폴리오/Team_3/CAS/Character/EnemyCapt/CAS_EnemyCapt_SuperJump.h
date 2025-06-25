// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/CAS_EnemyCapt.h"
#include "CAS_EnemyCapt_SuperJump.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API ACAS_EnemyCapt_SuperJump : public ACAS_EnemyCapt
{
	GENERATED_BODY()

public:

	ACAS_EnemyCapt_SuperJump();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ActivateAbility(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AbilityAction;
};
