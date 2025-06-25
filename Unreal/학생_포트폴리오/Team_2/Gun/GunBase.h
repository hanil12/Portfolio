// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GunBase.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FAmmoChanged, int, int);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMagChanged, int, int);

UENUM(BlueprintType)
enum class EGunType : uint8
{
	OneHanded,
	TwoHanded
};

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	FireAuto,
	FireSemi,
	FireBurst,
	FireBoltAction
};

UENUM(BlueprintType)
enum class EReloadStage : uint8
{
	None,
	RemoveMag,
	InsertMag,
	CloseBolt,
	RoundsReload // �� �߾� ����
};

UENUM(BlueprintType)
enum class ETacticalLightMode : uint8
{
	LightAuto,
	LightOn,
	LightOff
};

USTRUCT(BlueprintType)
struct FGunData // : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName _name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGunType _type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* _icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _baseDamage = 80.0f;

	// �߻� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _fireInterval = 60.0f / 640.0f;

	// ź��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 _maxAmmo = 45;

	// źâ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 _initialMag = 6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 _maxMag = 8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 _refillMagAmount = 4;

	// ��ü���м�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _ergo = 54;

	// �ݵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _recoil = 14.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _verticalRecoil = 5.f;        // ���� �ݵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _horizontalRecoil = 6.f;      // ���� �ݵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _shakeAmount = 4.f;           // ��鸲
	
	// �Ÿ��� ���� ������ ���ҷ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _falloff25 = 0.04f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _falloff50 = 0.072f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _falloff100 = 0.133f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<EFireMode> _fireModes = { EFireMode::FireAuto, EFireMode::FireBurst, EFireMode::FireSemi };
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ETacticalLightMode> _lightModes = {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> _scopeModes = {};
};

UCLASS()
class SGAPROJECTMAIN_API AGunBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGunBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartFire();
	virtual void Fire();
	virtual void StopFire();

	virtual void StartAiming();
	virtual void StopAiming();

	void InitializeGun();
	void ActivateGun();
	void DeactivateGun();
	void AttachToHand();

	void Reload();
	void FinishReload(class UAnimMontage* Montage, bool bInterrupted);
	void ChangeReloadStage(); // ���� ��Ÿ�� ���� ������ ȣ��
	void CancelReload();

	void RefillMag();

	float CalculateDamage(float distance); // �Ÿ��� ���� ������ ����

	//void TickRecoil(float DeltaTime);
	void RecoverRecoil(float DeltaTime); // �ݵ� ����
	void ApplyFireRecoil(); // ��ݿ� ���� �ݵ�
	float GetRecoilMultiplier(); // ���¿� ���� �ݵ� ����

	FHitResult GetHitResult();
	
	void ChangeFireMode();
	void ChangeTacticalLightMode();
	void ChangeScopeMode();

	void UseLaserPoint(FVector hitPoint);
	void UseTacticalLight(bool isAiming);

	void ResetCanFire() { _canFire = true; }

	FGunData GetGunData() { return _gunData; }
	int32 GetCurAmmo() { return _isChamberLoaded ? _curAmmo + 1 : _curAmmo; }
	EFireMode GetCurFireMode() { return _fireMode; }
	ETacticalLightMode GetCurLightMode() { return _tacticalLightMode; }
	int32 GetCurScopeMode() { return _scopeMode; }



	FAmmoChanged _ammoChanged;
	FTransform GetMuzzleTrans();
	FVector GetMuzzleLoc();
	FRotator GetMuzzleRot();
	FMagChanged _magChanged;

	FTransform GetLeftHandleTrans();
private:
	UPROPERTY(EditAnywhere, Category = "Game/Gun")
	TObjectPtr<USkeletalMeshComponent> _mesh;

	UPROPERTY(VisibleAnywhere, Category = "Game/Gun")
	class AHellDiver* _owner;

	UPROPERTY(EditAnywhere, Category = "Game/GunData")
	FGunData _gunData;


	bool _isActive = false;

	FTimerHandle _fireTimer;
	// ��Ʈ�׼ǿ�
	FTimerHandle _boltActionTimer;
	bool _canFire = true;

	int32 _curAmmo;
	int32 _curMag;

	FVector _hitPoint;

	FRotator _recoilToRecover = FRotator::ZeroRotator;
	// �ݵ� ���� ������ ���� ��ġ -> �׽�Ʈ �ʿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game/Recoil", meta = (AllowPrivateAccess = "true"))
	float _recoilRecoverSpeed = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game/Recoil", meta = (AllowPrivateAccess = "true"))
	float _verticalRecoilDamp = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game/Recoil", meta = (AllowPrivateAccess = "true"))
	float _horizontalRecoilDamp = 3.f;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game/Gun", meta = (AllowPrivateAccess = "true"))
	EReloadStage _reloadStage = EReloadStage::None;
	UPROPERTY(VisibleAnywhere, Category = "Game/Gun")
	bool _isChamberLoaded = false; // ��ǿ� ź�� ���Ҵ���

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game/Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* _reloadMontage;


	UPROPERTY()
	class AImpactMarker* _marker;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game/UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AImpactMarker> _impactMarkerClass;

	UPROPERTY()
	UUserWidget* _crosshair;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game/UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> _crosshairClass;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game/Gun", meta = (AllowPrivateAccess = "true"))
	EFireMode _fireMode = EFireMode::FireAuto;
	int32 _fireIndex = 0;
	int32 _burstCount = 3;

	UPROPERTY(VisibleAnywhere, Category = "Game/Gun")
	class USpotLightComponent* _tacticalLight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game/Gun", meta = (AllowPrivateAccess = "true"))
	ETacticalLightMode _tacticalLightMode = ETacticalLightMode::LightAuto;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game/Gun", meta = (AllowPrivateAccess = "true"))
	int32 _scopeMode;
	int32 _scopeIndex;

	UPROPERTY()
	class UNiagaraComponent* _laserpointer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game/Gun", meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* _laserFX;
	UPROPERTY()
	class UNiagaraComponent* _laserImpact;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Game/Gun", meta = (AllowPrivateAccess = "true"))
	class UNiagaraSystem* _laserImpactFX;
};
