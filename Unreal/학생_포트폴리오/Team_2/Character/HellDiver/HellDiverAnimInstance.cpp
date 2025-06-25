// Fill out your copyright notice in the Description page of Project Settings.


#include "HellDiverAnimInstance.h"
#include "HellDiver.h"
#include "../../Gun/GunBase.h"
#include "HellDiverStateComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimNode_StateMachine.h"
#include "Animation/AnimInstanceProxy.h"

#include "../../Gun/GunBase.h"

UHellDiverAnimInstance::UHellDiverAnimInstance()
{
	
}

void UHellDiverAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	auto pawn = TryGetPawnOwner();
	if (pawn)
	{
		_hellDiver = Cast<AHellDiver>(pawn);

	}
}

void UHellDiverAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (_hellDiver)
	{
		if (_hellDiver != nullptr)
		{
			_characterState= _hellDiver->GetStateComponent()->GetCharacterState();
			_actionState= _hellDiver->GetStateComponent()->GetActionState();
			_weaponState= _hellDiver->GetStateComponent()->GetWeaponState();
			_lifeState= _hellDiver->GetStateComponent()->GetLifeState();
			_isFiring= _hellDiver->GetStateComponent()->IsFiring();
			_isReloading= _hellDiver->GetStateComponent()->IsReloading();
			_isRolling= _hellDiver->GetStateComponent()->IsRolling();
			_isFocusing = _hellDiver->GetStateComponent()->IsFocusing();
			_leftHandTrans = _hellDiver->GetLeftHandSocketTransform();
			_jointTargetLoc = _hellDiver->GetJointTargetLocation();
			CheckEquipChange(_hellDiver->GetStateComponent()->GetEquipIndex());
			IsUsingLeftHand();
			IsUsingFocusing();
			AimFocus(DeltaSeconds);
			GetCurrentMoveNode();


		}
	}
}

void UHellDiverAnimInstance::GetCurrentMoveNode()
{
	FString temp1 = GetCurrentStateName(GetStateMachineIndex(TEXT("Move"))).ToString();
	FString temp2 = GetCurrentStateName(GetStateMachineIndex(TEXT("Look"))).ToString();
	MoveStateChanged(temp1);
	LookStateChanged(temp2);
}

void UHellDiverAnimInstance::AnimNotify_Reload()
{
	UE_LOG(LogTemp, Log, TEXT("ReloadNotify"));
	if (AGunBase* gun = Cast<AGunBase>(_hellDiver->GetEquippedGun()))
	{
		gun->ChangeReloadStage();
	}
}

bool UHellDiverAnimInstance::MoveStateChanged(FString curState)
{
	if (_currentMoveState==curState)
	{
		return false;
	}

	UE_LOG(LogTemp, Error, TEXT("Move : %s"), *curState);
	_currentMoveState = curState;
	if (_moveChanged.IsBound())
	{
		_moveChanged.Broadcast(curState);
	}
	return true;

}

bool UHellDiverAnimInstance::LookStateChanged(FString curState)
{
	if (_currentLookState == curState)
	{
		return false;
	}

	UE_LOG(LogTemp, Error, TEXT("Look : %s"), *curState);
	_currentLookState = curState;
	if (_lookChanged.IsBound())
	{
		_lookChanged.Broadcast(curState);
	}
	return true;
}

void UHellDiverAnimInstance::AimFocus(float DeltaSeconds)
{
	
	if(_useFocusing)
	{
		FRotator temp = _hellDiver->Focusing();
		double dot = temp.Roll;
		temp.Roll = 0.0f;
		_focusRotate += temp * DeltaSeconds * _rotateSpeed * dot;
		_focusAlpha = FMath::Clamp(_focusAlpha + _focusSpeed * DeltaSeconds, 0, 1);
	}
	else
	{
		// �ε巴�� ���� �ڼ��� ���ư�
		_focusRotate = FMath::RInterpTo(_focusRotate, FRotator::ZeroRotator, DeltaSeconds, _rotateSpeed);

		_focusAlpha = FMath::Clamp(_focusAlpha - _focusSpeed * DeltaSeconds, 0, 1);

	}
}

bool UHellDiverAnimInstance::IsStableState_Move()
{

	if (_currentMoveState == "Sprinting")
		return true;
	if (_currentLookState == "Crouching")
		return true;
	if (_currentMoveState == "Standing")
		return true;
	if (_currentLookState == "Proning")
		return true;
	return false;
}

bool UHellDiverAnimInstance::IsStableState_Look()
{
	if(_currentLookState=="UnArmed")
		return true;
	if (_currentLookState == "Gun")
		return true;
	return false;
}

bool UHellDiverAnimInstance::IsUsingLeftHand()
{
	auto gun=_hellDiver->GetEquippedGun();
	_useLeftHand = false;
	if (gun == nullptr)
		return false;
	if (_weaponState!=EWeaponType::Gun)
	{
		return false;
	}
	_useLeftHand = true;
	return true;
}

bool UHellDiverAnimInstance::IsUsingFocusing()
{
	_useFocusing = false;
	if (_hellDiver->GetStateComponent()->IsWeaponChanging())
		return false;
	if (!_isFocusing && IsMoving())
		return false;
	if (IsStableState_Look() == false)
		return false;
	if (_isReloading)
		return false;
	_useFocusing = true;
	return true;
}

void UHellDiverAnimInstance::CheckEquipChange(uint8 index)
{
	if (_curEquipIndex == index)
	{
		_changeWeapon = false;
		return;
	}
	_changeWeapon = true;
	_curEquipIndex = index;

}
