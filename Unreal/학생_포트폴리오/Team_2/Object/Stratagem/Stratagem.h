// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Throwable.h"
#include "Stratagem.generated.h"

/**
 * 
 */
UCLASS()
class SGAPROJECTMAIN_API AStratagem : public AThrowable
{
	GENERATED_BODY()
	
public:
	void DeployStratagem();

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	bool IsSurfaceAttachable(const FHitResult& Hit);

	UFUNCTION(BlueprintPure)
	const TArray<FKey>& GetInputSequence() const { return _inputSequence; }

	const FName GetStgName() const { return _name; }
	class UTexture2D* GetStgIcon() const { return _icon; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game/Stratagem")
	TSubclassOf<AActor> _objectToSpawn; // ���� ����/��Ʈ�� ��

	UPROPERTY(EditAnywhere, Category = "Game/Stratagem")
	float _deployDelay = 5.0f; // ��ô �� �� �� �ڿ� ����

	UPROPERTY(EditAnywhere, Category = "Game/Stratagem")
	bool _isAttackStratagem = false;

	UPROPERTY(EditDefaultsOnly, Category = "Game/Stratagem")
	TArray<FKey> _inputSequence;

	UPROPERTY(EditDefaultsOnly, Category = "Game/Stratagem")
	FName _name;

	UPROPERTY(EditDefaultsOnly, Category = "Game/Stratagem")
	class UTexture2D* _icon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game/Stratagem")
	AActor* _targetActor = nullptr;

	FTimerHandle _deployTimerHandle;
};
