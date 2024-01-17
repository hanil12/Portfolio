// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_PARAGON_API UMyHpBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Bind(class UMyStatComponent* statComponent);
	void SetHpBarRatio(float value);

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HPBar;
};
