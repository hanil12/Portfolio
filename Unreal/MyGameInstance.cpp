// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> dataTable(TEXT("DataTable'/Game/Data/StatTable.StatTable'"));

	_dataTable = dataTable.Object;
}

void UMyGameInstance::Init()
{
	Super::Init();

	FCharacterStat* stat = GetStatByLevel(1);
}

FCharacterStat* UMyGameInstance::GetStatByLevel(int32 level)
{
	FName name = *FString::FromInt(level);
	FCharacterStat* result = _dataTable->FindRow<FCharacterStat>(name, TEXT(""));

	return result;
}
