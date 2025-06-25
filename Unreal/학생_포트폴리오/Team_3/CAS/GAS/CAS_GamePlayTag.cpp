// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/CAS_GamePlayTag.h"

namespace CAS_GamePlayTag {
	
	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack_TEST, "Ability.Attack.TEST")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack_Capture, "Ability.Attack.Capture")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Attack_DeCapture, "Ability.Attack.DeCapture")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Move_Evade, "Ability.Move.Evade")
	UE_DEFINE_GAMEPLAY_TAG(Ability_State_Dead, "Ability.State.Dead")
	UE_DEFINE_GAMEPLAY_TAG(Ability_State_Invincible, "Ability.State.Invincible")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Move_SuperJump, "Ability.Move.SuperJump")
	UE_DEFINE_GAMEPLAY_TAG(Ability_Move_SuperSpeed, "Ability.Move.SuperSpeed")

	UE_DEFINE_GAMEPLAY_TAG(Effect_Attack_TEST, "Effect.Attack.TEST")
	UE_DEFINE_GAMEPLAY_TAG(Effect_Move_Evade, "Effect.Move.Evade")
	UE_DEFINE_GAMEPLAY_TAG(Effect_Move_SuperJump, "Effect.Move.SuperJump")
	UE_DEFINE_GAMEPLAY_TAG(Effect_Move_SuperSpeed, "Effect.Move.SuperSpeed")
	UE_DEFINE_GAMEPLAY_TAG(Effect_Status_Stun, "Effect.Status.Stun")

	UE_DEFINE_GAMEPLAY_TAG(State_TakeDamage, "State.TakeDamage")
	UE_DEFINE_GAMEPLAY_TAG(State_Nomal, "State.Nomal")
	UE_DEFINE_GAMEPLAY_TAG(State_Dead, "State.Dead")
	UE_DEFINE_GAMEPLAY_TAG(State_Invincible, "State.Invincible")
	UE_DEFINE_GAMEPLAY_TAG(State_Ability_Attacking, "State.Ability.Attacking")
		
}
