// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCapt/CAS_EnemyCapt_SuperJump.h"
#include "GameFramework/CharacterMovementComponent.h"


ACAS_EnemyCapt_SuperJump::ACAS_EnemyCapt_SuperJump()
{

}

void ACAS_EnemyCapt_SuperJump::BeginPlay()
{
	Super::BeginPlay();

	ACAS_Character::ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.Move.SuperJump"));

}

void ACAS_EnemyCapt_SuperJump::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Started, this, &ThisClass::Jump);

	}
}

void ACAS_EnemyCapt_SuperJump::ActivateAbility(const FInputActionValue& Value)
{
	ACAS_Character::ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.Move.SuperJump"));
}
