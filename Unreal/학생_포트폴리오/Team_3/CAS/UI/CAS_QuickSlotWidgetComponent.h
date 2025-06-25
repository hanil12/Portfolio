// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/CAS_SkillSlot.h"
#include "CAS_QuickSlotWidgetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CAS_API UCAS_QuickSlotWidgetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCAS_QuickSlotWidgetComponent();
public:
	void InitSetting(int32 count);
	
	bool AddPlayerAbility(int32 index, const TSubclassOf<class UGameplayAbility>& newAbility);
	void RemovePlayerAbility(int32 index);

	int32 FindEmptyPlayerAbilityIndex();

	FCAS_SlotData GetAbilityData(int32 index);
	void UpdateQuickSlot(int32 index, FCAS_SlotData slotData);
protected:

	UPROPERTY(VisibleAnywhere, Category = Abilities)
	TArray<FCAS_SlotData> PlayerAbilities;
};
