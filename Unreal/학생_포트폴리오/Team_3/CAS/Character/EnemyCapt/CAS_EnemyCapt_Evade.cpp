// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCapt/CAS_EnemyCapt_Evade.h"

ACAS_EnemyCapt_Evade::ACAS_EnemyCapt_Evade()
{
}

void ACAS_EnemyCapt_Evade::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	auto mesh = GetMesh();
	auto material = mesh->CreateDynamicMaterialInstance(0);
	auto material2 = mesh->CreateDynamicMaterialInstance(1);

	mesh->CreateDynamicMaterialInstance(0)->SetVectorParameterValue("Tint", FVector4(1, 0, 0, 1));
	mesh->CreateDynamicMaterialInstance(1)->SetVectorParameterValue("Tint", FVector4(1, 0, 0, 1));
}

void ACAS_EnemyCapt_Evade::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		EnhancedInputComponent->BindAction(AbilityAction, ETriggerEvent::Started, this, &ThisClass::ActivateAbility);

	}
}

void ACAS_EnemyCapt_Evade::ActivateAbility(const FInputActionValue& Value)
{
	ACAS_Character::ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.Move.Evade"));
}
