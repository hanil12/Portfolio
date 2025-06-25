// Fill out your copyright notice in the Description page of Project Settings.


#include "Stratagem.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../../Character/HellDiver/HellDiver.h"

void AStratagem::DeployStratagem()
{
	if (_objectToSpawn)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = Cast<APawn>(_owner); // �÷��̾ Instigator�� ����

		FVector targetLocation = GetActorLocation();

		// ���� ��ġ ����
		FVector dropOrigin;
		if (_isAttackStratagem)
		{
			dropOrigin = FVector(0, 0, 0); // �� �߾� ����
		}
		else
		{
			dropOrigin = targetLocation; // ��ȣ�� �ٷ� ��
		}

		// ���� ���� ��ġ = ������ + ���� ����
		FVector spawnLocation = dropOrigin + FVector(0, 0, 1500.f);

		// ���� = Ÿ�� - ���� ��ġ
		FVector direction = (targetLocation - spawnLocation).GetSafeNormal();
		FRotator spawnRotation = FRotator(0.0f, direction.Rotation().Yaw, 0.0f);

		// ����
		AActor* spawned = GetWorld()->SpawnActor<AActor>(
			_objectToSpawn,
			spawnLocation,
			spawnRotation,
			spawnParams
		);

		// �ӵ� ����
		if (spawned)
		{
			if (UProjectileMovementComponent* projectile = spawned->FindComponentByClass<UProjectileMovementComponent>())
			{
				projectile->Velocity = direction * 8000.f;
			}
		}
	}

	DestroySelf();
}

void AStratagem::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

	if (IsSurfaceAttachable(Hit))
	{
		// 1. �������� ȸ�� ���� (�ϴ� ������ ���ϰ�)
		FVector currentForward = GetActorForwardVector();
		FRotator uprightRotation = FRotationMatrix::MakeFromXZ(currentForward, FVector::UpVector).Rotator();
		SetActorRotation(uprightRotation);

		// 2. ���� ó�� (ȸ���� ������ ���� ���������Ƿ� ������)
		AttachToComponent(Hit.Component.Get(), FAttachmentTransformRules::KeepWorldTransform);

		// 3. �̵� ���� �� Ÿ�̸�
		_projectileMovement->StopMovementImmediately();
		GetWorldTimerManager().SetTimer(_deployTimerHandle, this, &AStratagem::DeployStratagem, _deployDelay, false);
	}
}

bool AStratagem::IsSurfaceAttachable(const FHitResult& Hit)
{
	// ���� ���� (0,0,1)�� �󸶳� �����Ѱ�
	float dot = FVector::DotProduct(Hit.ImpactNormal, FVector::UpVector);
	return dot > 0.5f; // 0.7 �̻��̸� ���� ���� ����� ���
}
