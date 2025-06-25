// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CAS_Ability_TESTFUNC.h"
#include "Ability_Task/CAS_Task_Attack.h"

UCAS_Ability_TESTFUNC::UCAS_Ability_TESTFUNC()
{		
	//공격당하면 이 어빌리티는 실행되지않음
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag("State.TakeDamage"));
}


bool UCAS_Ability_TESTFUNC::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	auto isPlaying = Cast<ACAS_Character>(ActorInfo->AvatarActor)->GetMesh()->GetAnimInstance()->Montage_IsPlaying(AttackMontage);
	if (isPlaying) {
		return false;
	}
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UCAS_Ability_TESTFUNC::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	/*
	행동(Task) 실행,효과(Effect) 적용

	해당 어빌리티 실행 과정
	1. 태스크에서 몽타주를재생
	2. 몽타주 애님 노티파이 델리게이트로 해당 시점의 적이 있는지 체크하는 함수 호출
	3. 적이 있다면 델리게이트로 어빌리티에 접근하여 타겟을 알려줌
	4. 타겟에 이펙트를 적용
	*/
	
	if (!AttackMontage || !DamageEffectClass) {
		return;
	}
	//1.
	/*
	정의할 행동과 기능
	예를들면 순간적으로 이동하여 공격을 한다면
	1. 적에게 텔레포트하는 몽타주
	2. 실제 텔레포트 하는 기능
	*/
	auto Task = UCAS_Task_Attack::Task_Attack(this, "TEST_Attack", AttackMontage, 1.5f);
	if (Task->IsValidLowLevel()) {
		Task->OnAttackHit.AddUObject(this, &ThisClass::ReceiveTarget);
		Task->OnAbilityEnd.AddUObject(this, &ThisClass::EndAbility);
		Task->ReadyForActivation();
	}
	

	//2.
	/*
	1.effect : 효과 클래스 그자체
	2.effect spec : 클래스의 세부내용 변경
	3.effect spec handle : 스펙으로 완성한 이펙트를 감싸는 포인터같은 개념 안전하게 참조하는 역할
	예를들면 적에게 공격을할때
	1. 이펙트는 공격하는 그 자체를 정의
	2. 이펙트에서 정의한 공격에 세부적인 데미지 정의 10 을 modifier에 전달
	3. 핸들을 통해 전달하여 적용
	*/
	

}

void UCAS_Ability_TESTFUNC::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UCAS_Ability_TESTFUNC::ApplyGamePlayEffect(ACAS_Character* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass, int32 GameplayEffectLevel, const FGameplayEffectContextHandle& EffectContext, UAbilitySystemComponent* AbilitySystemComponent)
{
	
	UAbilitySystemComponent* TargetAbilitySystemComp = Target->GetAbilitySystemComponent();

	//스펙을 클래스로 받아와서 만들어줌 1.0은 레벨지정 데이터테이블로 고렙 -> 고데미지 스킬로 만들수있음
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContext);
	if (SpecHandle.IsValid())
	{
		//스펙핸들에서 스펙을 뽑아와서 태그를 통해 키값을확인해서 데미지입력
		SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Effect.Attack.TEST")), -1.0f);
		AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetAbilitySystemComp);
	}

}

void UCAS_Ability_TESTFUNC::ReceiveTarget(ACAS_Character* Target, int32 TaskLevel)
{
	if (!DamageEffectClass || !TagEffectClass) {
		return;
	}
	auto PlayerState = Cast<ACAS_PlayerState>(GetOwningActorFromActorInfo());
	UAbilitySystemComponent* AbilitySystemComp = nullptr;
	if (PlayerState->IsValidLowLevel()) {
		AbilitySystemComp = PlayerState->GetAbilitySystemComponent();
	}
	else {
		auto CharacterState = Cast<ACAS_Character>(GetOwningActorFromActorInfo());
		if (CharacterState->IsValidLowLevel()) {
			AbilitySystemComp = CharacterState->GetAbilitySystemComponent();
		}
	}
	if (!AbilitySystemComp->IsValidLowLevel()) {
		return;
	}
	FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComp->MakeEffectContext();
	EffectContextHandle.AddInstigator(PlayerState, nullptr);

	ApplyGamePlayEffect(Target, DamageEffectClass, TaskLevel, EffectContextHandle, AbilitySystemComp);
	ApplyGamePlayEffect(Target, TagEffectClass, TaskLevel, EffectContextHandle, AbilitySystemComp);

}