// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CAS_Hpbar.generated.h"

/**
 * 
 */
UCLASS()
class CAS_API UCAS_Hpbar : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateHp(int32 count);
	void InitSetting(int32 count);
protected:
	virtual void NativeConstruct() override;
protected:
	UPROPERTY(Visibleanywhere, BlueprintReadOnly, meta = (BindWidget))
	class UHorizontalBox* CAS_HpBar;
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	int32 HpCount = 3;
	UPROPERTY(Editanywhere, BlueprintReadWrite)
	TSubclassOf<class UImage> HpBlock;

};
