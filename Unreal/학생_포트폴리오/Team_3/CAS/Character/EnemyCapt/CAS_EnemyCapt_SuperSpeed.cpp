// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCapt/CAS_EnemyCapt_SuperSpeed.h"

ACAS_EnemyCapt_SuperSpeed::ACAS_EnemyCapt_SuperSpeed()
{
}

void ACAS_EnemyCapt_SuperSpeed::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

	//	EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Started, this, &ThisClass::ActivateAbility);
	//}

}

//void ACAS_EnemyCapt_SuperSpeed::ActivateAbility(const FInputActionValue& Value)
//{
//	ACAS_Character::ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.Move.SuperSpeed"));
//}

void ACAS_EnemyCapt_SuperSpeed::BeginPlay()
{
	Super::BeginPlay();

	ACAS_Character::ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.Move.SuperSpeed"));

}
