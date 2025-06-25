// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StimPackComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FStimPackChanged, int, int);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SGAPROJECTMAIN_API UStimPackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStimPackComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UseStimPack();
	void RefillStimPack();

	// ȸ�� �ݹ�
	void HandleRegen();

	// ���� ����: Ÿ�̸� ����/����
	void StartOrExtendRegen();

	// ���� ���� ���� ��������Ʈ Broadcast
	void BroadcastStimPackChanged();
	FStimPackChanged _stimPackChanged;

private:
	UPROPERTY(VisibleAnywhere, Category = "Game/StimPack")
	class AHellDiver* _owner;

	UPROPERTY(EditDefaultsOnly, Category = "Game/StimPack")
	int32 _maxStimPack = 4;

	UPROPERTY(VisibleAnywhere, Category = "Game/StimPack")
	int32 _curStimPack = 2;

	FTimerHandle _regenTimerHandle;

	// ȸ�� ���ӽð�
	UPROPERTY(EditDefaultsOnly, Category = "Game/StimPack")
	float TotalRegenDuration = 4.0f;

	// ȸ�� ƽ ����
	UPROPERTY(EditDefaultsOnly, Category = "Game/StimPack")
	float RegenInterval = 0.05f;

	// ȸ�� ��
	UPROPERTY(EditDefaultsOnly, Category = "Game/StimPack")
	float AmountPerTick = 10.0f;

	// ���� ƽ ī����
	int32 _ticksElapsed = 0;
	int32 _maxTicks = 0;

	
};
