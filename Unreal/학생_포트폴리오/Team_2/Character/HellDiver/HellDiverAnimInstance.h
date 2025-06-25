// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CharacterAnimInstance.h"
#include "HellDiverStateComponent.h"
#include "HellDiverAnimInstance.generated.h"

/*
	�ͻ�𿡼� �߰��ؿ� �ִϸ��̼�
	Taunt : �Ϻθ� �����ͼ� ��Ʈ��Ÿ���� ������ �ִϸ��̼����� ����� �� �����Ͱ���
	Flying Death : �Ϻθ� �����ͼ� �ڷ� Rolling�Ҷ� ����� �� �����Ͱ���
	GoalKeeper Diving Safe : �Ϻθ� �����ͼ� ������ Rolling�Ҷ� ����� �� ������ ����.


*/


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveAnimStateChanged, FString, newState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLookAnimStateChanged, FString, newState);

UCLASS()
class SGAPROJECTMAIN_API UHellDiverAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()
public:
	UHellDiverAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void GetCurrentMoveNode();

	UFUNCTION()
	void AnimNotify_Reload();

	FOnMoveAnimStateChanged _moveChanged;
	FOnLookAnimStateChanged _lookChanged;

protected:
	bool MoveStateChanged(FString curState);
	bool LookStateChanged(FString curState);

	void AimFocus(float DeltaSeconds);
	bool IsStableState_Move();
	bool IsStableState_Look();
	bool IsUsingLeftHand();
	bool IsUsingFocusing();
	void CheckEquipChange(uint8 index);
protected:

	class AHellDiver* _hellDiver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ECharacterState _characterState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EActionState _actionState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EWeaponType _weaponState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	ELifeState _lifeState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FRotator _focusRotate=FRotator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float _focusAlpha = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float _focusSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool _useLeftHand = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool _changeWeapon = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FTransform _leftHandTrans = FTransform();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FVector _jointTargetLoc = FVector();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	uint8 _curEquipIndex=0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool _useLeftHandIk = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool _useFocusing = false;
	float _rotateSpeed = 350;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool _isFocusing = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool _isFiring = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool _isReloading = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool _isRolling = false;


	FString _currentMoveState;
	FString _currentLookState;



};
