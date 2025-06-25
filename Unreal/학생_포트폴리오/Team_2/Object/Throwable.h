// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Throwable.generated.h"

UCLASS()
class SGAPROJECTMAIN_API AThrowable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void AttachToHand(FName socketName);

	virtual void Throw(FVector direction); // ������
	virtual void OnLifeTimeExpired(); // ���� ����

	UStaticMeshComponent* GetMesh() const { return _mesh; }
	void DestroySelf();

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp,AActor* OtherActor,	UPrimitiveComponent* OtherComp,	FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game/Throwable")
	UStaticMeshComponent* _mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game/Throwable")
	class UProjectileMovementComponent* _projectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game/Throwable")
	float _lifeTime = 10.0f; //���� ���ű��� ���� �ð�

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game/Throwable")
	class AHellDiver* _owner;

	FTimerHandle _destroyTimerHandle;

};
