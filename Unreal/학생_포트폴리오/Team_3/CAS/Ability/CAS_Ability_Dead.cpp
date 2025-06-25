// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_Dead.h"
#include "Ability_Task/CAS_Task_Dead.h"

UCAS_Ability_Dead::UCAS_Ability_Dead()
{
}

bool UCAS_Ability_Dead::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	auto isPlaying = Cast<ACAS_Character>(ActorInfo->AvatarActor)->GetMesh()->GetAnimInstance()->Montage_IsPlaying(DeadMontage);
	if (isPlaying) {
		return false;
	}
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UCAS_Ability_Dead::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!DeadMontage) {
		return;
	}
	auto Task = UCAS_Task_Dead::Task_Dead(this, "TEST_Attack", DeadMontage, 1.0f);
	if (Task->IsValidLowLevel()) {
		Task->OnAbilityEnd.AddUObject(this, &ThisClass::EndAbility);
		Task->ReadyForActivation();
	}
}

void UCAS_Ability_Dead::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

}

/*
Todo
체력이 0되면 델리게이트로 플레이어의 데드어빌리티 실행
데드어빌리티 -> 즉시 데드 태그를 달고
->태스크실행 ->애님노티파이로 사라지게처리->어빌리티 종료

데드태그 -> 행동트리에서 막아야함 , 데드태그를 달면 하던공격취소 및 데미지 x


*/