// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/CAS_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

void ACAS_PlayerController::BeginPlay()
{
	Super::BeginPlay();
#if WITH_EDITOR
    if (IsLocalController())
    {
        ConsoleCommand(TEXT("ShowDebug AbilitySystem"), true);
    }
#endif


}

void ACAS_PlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(_inputMappingContext, 0);
    }
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {

		EnhancedInputComponent->BindAction(DebugAction, ETriggerEvent::Started, this, &ThisClass::PrintDebugMessage);

	}
}

void ACAS_PlayerController::PrintDebugMessage(const FInputActionValue& Value)
{
    if (IsLocalController())
    {
        ConsoleCommand(TEXT("ShowDebug AbilitySystem"), true);
    }
}
