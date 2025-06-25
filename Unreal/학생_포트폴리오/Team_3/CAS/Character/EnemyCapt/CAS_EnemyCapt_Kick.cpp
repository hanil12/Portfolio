// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCapt/CAS_EnemyCapt_Kick.h"

ACAS_EnemyCapt_Kick::ACAS_EnemyCapt_Kick()
{
}

void ACAS_EnemyCapt_Kick::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Started, this, &ThisClass::ActivateAbility);
	}

}

void ACAS_EnemyCapt_Kick::ActivateAbility(const FInputActionValue& Value)
{
	ACAS_Character::ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.Attack.Test"));
}
