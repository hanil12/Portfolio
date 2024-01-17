// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHpBarWidget.h"
#include "MyStatComponent.h"
#include "Components/ProgressBar.h"

void UMyHpBarWidget::Bind(UMyStatComponent* statComponent)
{
	statComponent->_onHpChangedRatio.AddUObject(this, &UMyHpBarWidget::SetHpBarRatio);
}

void UMyHpBarWidget::SetHpBarRatio(float value)
{
	PB_HPBar->SetPercent(value);
}
