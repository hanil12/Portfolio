// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/Monster_Epic01_Anim.h"

UMonster_Epic01_Anim::UMonster_Epic01_Anim()
{

	// 근접공격 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Monster_Epic_01_AttackNear
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Animation/Monster/EpicMonster/EpicMonster01_AnimMontage_Near.EpicMonster01_AnimMontage_Near'"));

	if (Monster_Epic_01_AttackNear.Succeeded())
	{
		_myAnimMontage = Monster_Epic_01_AttackNear.Object;
	}

	// 원거리공격, 도트마법 몽타주
	static ConstructorHelpers::FObjectFinder<UAnimMontage> Monster_Epic_01_AttackFar
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Animation/Monster/EpicMonster/EpicMonster01_AnimMontage_Far.EpicMonster01_AnimMontage_Far'"));

	if (Monster_Epic_01_AttackFar.Succeeded())
	{
		_attackFarMontage = Monster_Epic_01_AttackFar.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> Monster_Epic_01_AttackMagic
	(TEXT("/Script/Engine.AnimMontage'/Game/Blueprint/Animation/Monster/EpicMonster/EpicMonster01_AnimMontage_Far.EpicMonster01_AnimMontage_Far2'"));

	if (Monster_Epic_01_AttackMagic.Succeeded())
	{
		_attackMagicMontage = Monster_Epic_01_AttackMagic.Object;
	}

}


void UMonster_Epic01_Anim::AnimNotify_Attackhit()
{
	_attackDelegate.Broadcast();
}

void UMonster_Epic01_Anim::AnimNotify_Death()
{
	_death_Epic_MonsterDelegate.Broadcast();
}

void UMonster_Epic01_Anim::PlayAttackFarMontage()
{
	Super::PlayAttackFarMontage();
}

void UMonster_Epic01_Anim::PlayAttackDotrMontage()
{
	Super::PlayAttackDotrMontage();
}
