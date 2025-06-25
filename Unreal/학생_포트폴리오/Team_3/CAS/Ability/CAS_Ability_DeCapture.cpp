// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_DeCapture.h"
#include "Ability_Task/CAS_Task_DeCapture.h"
UCAS_Ability_DeCapture::UCAS_Ability_DeCapture()
{
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("State.TakeDamage"));
}

bool UCAS_Ability_DeCapture::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	auto isPlaying = Cast<ACAS_Character>(ActorInfo->AvatarActor)->GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontage);
	if (isPlaying) {
		return false;
	}
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UCAS_Ability_DeCapture::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	auto Task = UCAS_Task_DeCapture::Task_DeCapture(this, "DeCapture");
	if (Task->IsValidLowLevel()) {
		Task->OnAbilityEnd.AddUObject(this, &ThisClass::EndAbility);
		Task->ReadyForActivation();
	}

	auto owner = Cast<ACAS_Character>(GetGameplayTaskAvatar(Task));

	StunTarget(owner, 1);

}

void UCAS_Ability_DeCapture::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCAS_Ability_DeCapture::ApplyGamePlayEffect(ACAS_Character* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel, const FGameplayEffectContextHandle& EffectContext, UAbilitySystemComponent* AbilitySystemComponent)
{
	UAbilitySystemComponent* TargetAbilitySystemComp = Target->GetAbilitySystemComponent();

	//스펙을 클래스로 받아와서 만들어줌 1.0은 레벨지정 데이터테이블로 고렙 -> 고데미지 스킬로 만들수있음
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContext);
	if (SpecHandle.IsValid())
	{
		//스펙핸들에서 스펙을 뽑아와서 태그를 통해 키값을확인해서 데미지입력
		//SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Effect.Attack.TEST")), -1.0f);
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetAbilitySystemComp);
	}
}

void UCAS_Ability_DeCapture::StunTarget(ACAS_Character* Target, int32 TaskLevel)
{
	if (!TagEffectClass) {
		return;
	}

	UAbilitySystemComponent* AbilitySystemComp = nullptr;

	auto CharacterState = Cast<ACAS_Character>(GetOwningActorFromActorInfo());
	if (CharacterState->IsValidLowLevel()) {
		AbilitySystemComp = CharacterState->GetAbilitySystemComponent();
	}

	if (!AbilitySystemComp->IsValidLowLevel()) {
		return;
	}
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComp->MakeEffectContext();

	ApplyGamePlayEffect(Target, TagEffectClass, TaskLevel, EffectContextHandle, AbilitySystemComp);
}
