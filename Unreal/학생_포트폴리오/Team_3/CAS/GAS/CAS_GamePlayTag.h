// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NativeGameplayTags.h"


namespace CAS_GamePlayTag {
	
	//Ability
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack_TEST);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack_Capture);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack_DeCapture);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Move_Evade);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Move_SuperJump);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Move_SuperSpeed);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_Dead);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_Invincible);
	


	//Effect
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Attack_TEST);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Move_Evade);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Move_SuperJump);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Move_SuperSpeed);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Status_Stun);

	//StateState_TakeDamage
	UE_DECLARE_GAMEPLAY_TAG_EXTERN();
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Nomal);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Ability_Attacking);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dead);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Invincible);
	
	//UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Ability_Attacking);

}