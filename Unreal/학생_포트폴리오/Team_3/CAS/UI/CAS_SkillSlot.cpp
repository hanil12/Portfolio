// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_SkillSlot.h"
#include "UI/CAS_QuickSlotWidget.h"
#include "Components/Image.h"
#include "Character/CAS_Player.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Blueprint/DragDropOperation.h"

void UCAS_SkillSlot::SetSlotData(const FCAS_SlotData& Data)
{
	DragSlotData = Data;
	UpdateIcon();
}

void UCAS_SkillSlot::UpdateIcon()
{
	UTexture2D* Texture;
	if (DragSlotData.SlotTexture->IsValidLowLevel())
	{
		Texture = DragSlotData.SlotTexture;
	}
	else {
		Texture = DefaultTexture;
	}
	CAS_Image->SetBrushFromTexture(Texture);
}

FReply UCAS_SkillSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		auto player = Cast<ACAS_Player>(GetOwningPlayerPawn());
		UCAS_QuickSlotWidget* quickSlotWidget = Cast<UCAS_QuickSlotWidget>(player->GetQuickSlotWidget());
		if (quickSlotWidget->IsValidLowLevel()) {
			quickSlotWidget->RemoveAbilityEvent.Broadcast(this->GetSlotIndex());
		}

	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UCAS_SkillSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	UDragDropOperation* DragOp = NewObject<UDragDropOperation>();

	UCAS_SkillSlot* DragVisual = CreateWidget<UCAS_SkillSlot>(GetOwningPlayer(), SkillSlotWidgetClass);

	DragVisual->SetRenderOpacity(0.7f);

	if (CAS_Image && CAS_Image->Brush.GetResourceObject())
	{
		UTexture2D* Texture = Cast<UTexture2D>(CAS_Image->Brush.GetResourceObject());
		if (Texture->IsValidLowLevel())
		{
			DragVisual->CAS_Image->SetBrushFromTexture(Texture, false);
			DragVisual->CAS_Image->SetDesiredSizeOverride(FVector2D(128.f, 128.f));
		}
	}

	DragOp->Payload = this;
	DragOp->DefaultDragVisual = DragVisual;
	OutOperation = DragOp;
}

bool UCAS_SkillSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool result = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if (auto otherSlot = Cast<UCAS_SkillSlot>(InOperation->Payload))
	{
		auto player = Cast<ACAS_Player>(GetOwningPlayerPawn());
		
		UCAS_QuickSlotWidget* quickSlotWidget = Cast<UCAS_QuickSlotWidget>(player->GetQuickSlotWidget());
		if (quickSlotWidget->IsValidLowLevel())
		{
			result = true;
			quickSlotWidget->SwapSlots(otherSlot, this);
		}
	}
	return result;
}
