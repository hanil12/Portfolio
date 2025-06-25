// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_QuickSlotWidget.h"
#include "UI/CAS_SkillSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
void UCAS_QuickSlotWidget::InitSetting(int32 count)
{
    SlotCount = count;

    SkillSlots.SetNum(SlotCount);

    for (int32 i = 0; i < SlotCount;i++)
    {
        if (SlotWidgetClass)
        {
            UCAS_SkillSlot* slot = CreateWidget<UCAS_SkillSlot>(GetWorld(), SlotWidgetClass);
            FCAS_SlotData Data;
            Data.SlotIndex = i;
            Data.SlotTexture = nullptr;
            Data.AbilityTag = FName(TEXT("None"));

            slot->SetSlotData(Data);

            SkillSlots[i] = slot;
            if (SlotBox->IsValidLowLevel())
            {
                SlotBox->AddChildToHorizontalBox(slot);
            }
        }
    }

}

void UCAS_QuickSlotWidget::SwapSlots(UCAS_SkillSlot* DragSlot, UCAS_SkillSlot* DropSlot)
{
    int32 DragIndex = DragSlot->GetSlotIndex();
    int32 DropIndex = DropSlot->GetSlotIndex();

    if (DragIndex == DropIndex) {
        return;
    }
    FCAS_SlotData DragSlotData = DragSlot->GetSlotData();
    FCAS_SlotData DropSlotData = DropSlot->GetSlotData();

    DragSlotData.SlotIndex = DropIndex;
    DropSlotData.SlotIndex = DragIndex;

    DragSlot->SetSlotData(DropSlotData);
    DropSlot->SetSlotData(DragSlotData);

    QuickSlotSwapEvent.Broadcast(DragIndex, DropSlotData);
    QuickSlotSwapEvent.Broadcast(DropIndex, DragSlotData);

}

void UCAS_QuickSlotWidget::SetSlotData(int32 index, const FCAS_SlotData& AbilityData)
{
    SkillSlots[index]->SetSlotData(AbilityData);
}

void UCAS_QuickSlotWidget::RemoveSlotData(int32 index)
{
    FCAS_SlotData Data;
    Data.SlotIndex = index;
    Data.SlotTexture = nullptr;
    Data.AbilityTag = FName(TEXT("None"));

    SkillSlots[index]->SetSlotData(Data);
}
