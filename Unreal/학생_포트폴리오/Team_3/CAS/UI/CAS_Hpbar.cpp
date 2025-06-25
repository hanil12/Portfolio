// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CAS_Hpbar.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h" 
#include "Blueprint/WidgetTree.h"

void UCAS_Hpbar::UpdateHp(int32 count)
{
    CAS_HpBar->ClearChildren();
    
    for (int32 i = 0; i < count; i++)
    {
        UWidgetTree* WidgetTreePtr = WidgetTree.Get();
        UImage* hpBlock = WidgetTreePtr->ConstructWidget<UImage>(HpBlock);
        if (hpBlock)
        {
            CAS_HpBar->AddChildToHorizontalBox(hpBlock);
        }
    }

}

void UCAS_Hpbar::InitSetting(int32 count)
{
    HpCount = count;
    for (int32 i = 0; i < HpCount; i++)
    {
        UWidgetTree* WidgetTreePtr = WidgetTree.Get();
        UImage* hpBlock = WidgetTreePtr->ConstructWidget<UImage>(HpBlock);
        if (hpBlock)
        {
            CAS_HpBar->AddChildToHorizontalBox(hpBlock);
        }
    }

}

void UCAS_Hpbar::NativeConstruct()
{
	Super::NativeConstruct();
    
}
