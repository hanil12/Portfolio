// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_QuickSlotWidgetComponent.h"
#include "Character/CAS_Player.h"
#include "GAS/CAS_GameplayAbility.h"

// Sets default values for this component's properties
UCAS_QuickSlotWidgetComponent::UCAS_QuickSlotWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCAS_QuickSlotWidgetComponent::InitSetting(int32 count)
{
	PlayerAbilities.SetNum(count);

	for (int32 i = 0; i < count; i++) {
		FCAS_SlotData Data;
		Data.SlotIndex = i;
		Data.SlotTexture = nullptr;
		Data.AbilityTag = FName(TEXT("None"));
		PlayerAbilities[i] = Data;
	}
}

bool UCAS_QuickSlotWidgetComponent::AddPlayerAbility(int32 index,const TSubclassOf<class UGameplayAbility>& newAbility)
{
	auto player = Cast<ACAS_Player>(GetOwner());
	if (player->IsValidLowLevel()) {
		auto ASC = Cast<UCAS_AbilitySystemComponent>(player->GetAbilitySystemComponent());
		if (ASC->FindAbilitySpecFromClass(newAbility) == nullptr) {
			auto AbilitySpec = FGameplayAbilitySpec(newAbility);
			ASC->GiveAbility(AbilitySpec);
			
			auto DefaultObj = newAbility->GetDefaultObject<UCAS_GameplayAbility>();

			if (DefaultObj->IsValidLowLevel()) {
				UTexture2D* TextureData = DefaultObj->AbilityIcon;
				FName TagData = DefaultObj->AbilityTags.GetByIndex(0).GetTagName();

				FCAS_SlotData Data;
				Data.SlotIndex = index;

				Data.SlotTexture = TextureData;
				Data.AbilityTag = TagData;

				PlayerAbilities[index] = Data;
				return true;
			}
			
		}
	}
	return false;
}

void UCAS_QuickSlotWidgetComponent::RemovePlayerAbility(int32 index)
{
	FCAS_SlotData slotData = PlayerAbilities[index];
	if (slotData.SlotTexture == nullptr) {
		return;
	}

	auto player = Cast<ACAS_Player>(GetOwner());
	if (player->IsValidLowLevel()) {
		auto ASC = Cast<UCAS_AbilitySystemComponent>(player->GetAbilitySystemComponent());
		
		FName tagName = slotData.AbilityTag;

		ASC->RemoveAbility(FGameplayTag::RequestGameplayTag(tagName));

		slotData.SlotIndex = index;
		slotData.SlotTexture = nullptr;
		slotData.AbilityTag = FName(TEXT("None"));

		PlayerAbilities[index] = slotData;
	}
}

FCAS_SlotData UCAS_QuickSlotWidgetComponent::GetAbilityData(int32 index)
{
	return PlayerAbilities[index];
}

void UCAS_QuickSlotWidgetComponent::UpdateQuickSlot(int32 index, FCAS_SlotData slotData)
{
	PlayerAbilities[index] = slotData;
}

int32 UCAS_QuickSlotWidgetComponent::FindEmptyPlayerAbilityIndex()
{
	int32 indexCount = 0;
	for (auto& playerAbility : PlayerAbilities) {
		if (playerAbility.SlotTexture == nullptr) {
			return indexCount;
		}
		else {
			indexCount++;
		}
	}

	return -1;
}

