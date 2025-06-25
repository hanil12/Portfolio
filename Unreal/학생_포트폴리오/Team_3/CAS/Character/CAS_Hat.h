// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CAS_Hat.generated.h"

UCLASS()
class CAS_API ACAS_Hat : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACAS_Hat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnMyCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult);

	UFUNCTION()
	void Throw(const FVector& direction);

	UFUNCTION()
	void ThrowAndReturn(float DeltaTime);

	UFUNCTION()
	void Return();

	UFUNCTION()
	void SetPlayer(class ACAS_Player* player) { _player = player; }

	UFUNCTION()
	class ACAS_Player* GetPlayer() { return _player; }

private:
	UPROPERTY(EditAnywhere, Category = "Mesh")
	class UStaticMeshComponent* _mesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	class USphereComponent* _collider;

	UPROPERTY(VisibleAnywhere, Category = "TestCaptureTarget")
	class ACAS_EnemyCapt* _testCaptureTarget;

	UPROPERTY(VisibleAnywhere, Category = "Player")
	class ACAS_Player* _player;

	FVector StartLocation;
	FVector TargetLocation;
	FVector MoveDirection;

	bool _isThrowing = false;
	bool _isReturning = false;
	float _capturingTime = 0.0f;
	float _totalMoveTime = 1.0f;
};
