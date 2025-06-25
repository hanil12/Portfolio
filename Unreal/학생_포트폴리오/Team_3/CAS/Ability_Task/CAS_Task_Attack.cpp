// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_Attack.h"
#include "Character/CAS_Character.h"

UCAS_Task_Attack* UCAS_Task_Attack::Task_Attack(UGameplayAbility* OwningAbility, FName TaskName, UAnimMontage* MontageToPlay, float Rate)
{
	if (!MontageToPlay) {
		return nullptr;
	}
	UCAS_Task_Attack* Task = NewAbilityTask<UCAS_Task_Attack>(OwningAbility, TaskName);

	Task->Montage = MontageToPlay;
	Task->PlayRate = Rate;

	return Task;
}

void UCAS_Task_Attack::Activate()
{
	Super::Activate();

	if (!Montage) {
		return;
	}
	auto Character = Cast<ACAS_Character>((GetAvatarActor()));
	if (Character->IsValidLowLevel()) {

		UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

		if (!AnimInstance)
		{
			return;
		}
	
		AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ThisClass::PlayAnimNotify);
		AnimInstance->Montage_Play(Montage, PlayRate);		
	}
}

void UCAS_Task_Attack::PlayAnimNotify(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	FHitResult hitResult;
	FCollisionQueryParams params(FName("Task_Attack"), false, GetAvatarActor());

	FVector forward = GetAvatarActor()->GetActorForwardVector();
	FQuat quat = FQuat::FindBetweenVectors(FVector(0, 0, 1), forward);

	float attackRadius = 100.0f;
	float attackRange = 100.0f;

	FVector Center = GetAvatarActor()->GetActorLocation() + forward * attackRange * 0.5f;
	FVector Start = GetAvatarActor()->GetActorLocation();
	FVector End = GetAvatarActor()->GetActorLocation() + forward * attackRange;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT hitResult,
		Start,
		End,
		quat,
		ECC_GameTraceChannel4,
		FCollisionShape::MakeCapsule(attackRadius, attackRange * 0.5f),
		params
	);

	FColor drawColor = FColor::Green;

	if (bResult && hitResult.GetActor()->IsValidLowLevel())
	{
		drawColor = FColor::Red;
		ACAS_Character* victim = Cast<ACAS_Character>(hitResult.GetActor());
		if (victim) {
			
			//FVector hitPoint = hitResult.ImpactPoint;			
			OnAttackHit.Broadcast(victim,1);
		}
	}
	

	DrawDebugCapsule(
		GetWorld(),
		Center,
		attackRange * 0.5f,
		attackRadius,
		quat,
		drawColor,
		false,
		1.0f
	);

	auto Character = Cast<ACAS_Character>((GetAvatarActor()));
	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(this, &ThisClass::PlayAnimNotify);

	auto Handle = Ability->GetCurrentAbilitySpecHandle();
	auto ActorInfo = Ability->GetCurrentActorInfo();
	auto ActivationInfo = Ability->GetCurrentActivationInfo();

	OnAbilityEnd.Broadcast(Handle, ActorInfo, ActivationInfo, true, false);
}


