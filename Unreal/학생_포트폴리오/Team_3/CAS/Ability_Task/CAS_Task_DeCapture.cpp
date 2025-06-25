// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Task/CAS_Task_DeCapture.h"
#include "Character/CAS_EnemyCapt.h"
#include "Character/CAS_Player.h"
#include "Character/CAS_Hat.h"
#include "AIController.h"
#include "Math/UnrealMathUtility.h"


UCAS_Task_DeCapture* UCAS_Task_DeCapture::Task_DeCapture(UGameplayAbility* OwningAbility, FName TaskName)
{
	UCAS_Task_DeCapture* Task = NewAbilityTask<UCAS_Task_DeCapture>(OwningAbility, TaskName);

	return Task;
}

void UCAS_Task_DeCapture::Activate()
{

	auto owner = Cast<ACAS_EnemyCapt>(GetAvatarActor());

	if (!owner->IsValidLowLevel())
		return;

	if (!owner->IsCaptured())
		return;

	owner->ActivateCapture(false);
	auto player = owner->GetHat()->GetPlayer();

	auto controller = Cast<APlayerController>(owner->GetController());
	controller->UnPossess();

	controller->Possess(player);

	int32 curHp = owner->GetAttributeSet()->GetHealth();

	player->SetHp(curHp);

	////디버그용으로 빙의해제되면 체력이 1 남음
	owner->SetHp(1);

	auto iter = GetWorld()->GetControllerIterator();
	for (int i = 0; i < GetWorld()->GetNumControllers(); i++)
	{
		if ((*iter)->GetPawn() != nullptr)
		{
			iter++;
			continue;
		}
		break;
	}

	auto aiController = Cast<AAIController>(*iter);
	aiController->Possess(owner);

	FVector acotrLocation = owner->GetActorLocation();

	float dropRadius = 300.0f;
	FVector randomOffset = FMath::VRand() * FMath::FRandRange(200.0f, dropRadius);
	FVector dropLocation = acotrLocation + randomOffset;
	dropLocation.Z = acotrLocation.Z;

	player->SetActorLocation(dropLocation);
	player->SetActorHiddenInGame(false);
	player->SetActorEnableCollision(true);


	owner->GetHat()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	owner->GetHat()->Return();
	owner->ClearHat();
	//owner->BeStunned();

	auto Handle = Ability->GetCurrentAbilitySpecHandle();
	auto ActorInfo = Ability->GetCurrentActorInfo();
	auto ActivationInfo = Ability->GetCurrentActivationInfo();

	OnAbilityEnd.Broadcast(Handle, ActorInfo, ActivationInfo, true, false);

}
