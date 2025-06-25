// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

#include "Components/Button.h"

AMainPlayerController::AMainPlayerController()
{
}

void AMainPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMainPlayerController::BeginPlay()
{
	//�θ��� BeginPlay ȣ��. ��ӱ����� �̺�Ʈ�� �θ��� �̺�Ʈ�� ȣ���������
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* subSystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (subSystem)
	{
		subSystem->AddMappingContext(_inputMappingContext, 0);
	}
}

void AMainPlayerController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
}

void AMainPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}
