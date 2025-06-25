// Fill out your copyright notice in the Description page of Project Settings.

#include "CAS_Character.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "GAS/CAS_GameplayAbility.h"
#include "UI/CAS_Hpbar.h"
#include "Controller/CAS_PlayerController.h"
#include "Controller/CAS_EnemyController.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACAS_Character::ACAS_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	AbilitySystemComponent = CreateDefaultSubobject<UCAS_AbilitySystemComponent>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UCAS_AttributeSet>("PlayerAttributeSet");

	HpBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	HpBarWidgetComponent->SetupAttachment(RootComponent);

	HpBarWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	
	HpBarWidgetComponent->SetCollisionProfileName("NoCollision");

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/CAS/Blueprint/UI/HpBar/BP_HpBar.BP_HpBar_C'"));
	
	if (WidgetClass.Succeeded())
	{
		HpBarWidgetClass = WidgetClass.Class;
	}
	static ConstructorHelpers::FClassFinder<UGameplayAbility> DeadAbilityClass(TEXT("/Script/Engine.Blueprint'/Game/CAS/Blueprint/Ability/GA_Ability_Dead.GA_Ability_Dead_C'"));

	if (DeadAbilityClass.Succeeded())
	{
		DefaultAbilities.Add(DeadAbilityClass.Class);
	}
}

// Called when the game starts or when spawned
void ACAS_Character::BeginPlay()
{
	Super::BeginPlay();
	if(HpBarWidgetClass){
	
		HpBarWidgetComponent->SetWidgetClass(HpBarWidgetClass);
		auto widget = Cast<UCAS_Hpbar>(HpBarWidgetComponent->GetWidget());
		if (widget) { 
			widget->InitSetting(HpCount);			
		}
		HpBarWidgetComponent->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 200.0f), FRotator::ZeroRotator);
		HpBarWidgetComponent->SetCastShadow(false);
	}
	if (AttributeSet) {
		AttributeSet->SetHealth(HpCount);
		AttributeSet->DeadEvent.AddUObject(this, &ThisClass::DeadEvent);
		auto widget = Cast<UCAS_Hpbar>(HpBarWidgetComponent->GetWidget());
		if (widget) {
			AttributeSet->HpChanged.AddUObject(widget,&UCAS_Hpbar::UpdateHp);
		}		

		AttributeSet->SpeedChanged.AddUObject(this, &ThisClass::SetWalkSpeed);
		//AttributeSet->JumpForceChanged.AddUObject(this, &ThisClass::SetJumpForce);
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AttributeSet->GetJumpForceAttribute()).AddUObject(this, &ACAS_Character::OnJumpForceChanged);


	//AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetJumpForceAttribute().AddUObject(this, &ACAS_Character::SetJumpForce));
}

void ACAS_Character::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilitySystemComponent(NewController);
	AddDefaultAbilites();
}

void ACAS_Character::DeadEvent()
{
	ActivateAbility(FGameplayTag::RequestGameplayTag("Ability.State.Dead"));

}

// Called every frame
void ACAS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto playerCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;

	if (playerCameraManager)
	{
		FVector hpBarLocation = HpBarWidgetComponent->GetComponentLocation();
		FVector cameraLocation = playerCameraManager->GetCameraLocation();
		FRotator rotation = UKismetMathLibrary::FindLookAtRotation(hpBarLocation, cameraLocation);
		HpBarWidgetComponent->SetWorldRotation(rotation);
	}
}

UAbilitySystemComponent* ACAS_Character::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}


void ACAS_Character::AddDefaultAbilites()
{
	UCAS_AbilitySystemComponent* ASC = Cast<UCAS_AbilitySystemComponent>(AbilitySystemComponent);
	if (!ASC) {
		return;
	}
	ASC->AddCharacterAbilities(DefaultAbilities);
}

void ACAS_Character::InitAbilitySystemComponent(AController* controller)
{
	if (auto curController = Cast<ACAS_PlayerController>(controller)) {
		auto playerState = Cast<ACAS_PlayerState>(GetPlayerState());
		int32 curHp = playerState->GetAttributeSet()->GetHealth();
		SetHp(curHp);
	}
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Nomal"));
}

void ACAS_Character::SetHp(int32 value)
{
	AttributeSet->SetHealth(value);
	AttributeSet->HpChanged.Broadcast(value);
}

void ACAS_Character::SetWalkSpeed(int32 value)
{
	GetCharacterMovement()->MaxWalkSpeed = value;

}

void ACAS_Character::SetJumpForce(float value)
{
	GetCharacterMovement()->JumpZVelocity = value;

}

void ACAS_Character::OnJumpForceChanged(const FOnAttributeChangeData& Data)
{
	SetJumpForce(Data.NewValue); // JumpZVelocity 반영
}



void ACAS_Character::ActivateAbility(const FGameplayTag tag)
{
	AbilitySystemComponent->ActivateAbility(tag);
}

void ACAS_Character::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UCAS_GameplayAbility*>& Abilities)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetActiveAbilitiesWithTags(AbilityTags, Abilities);
	}
}

bool ACAS_Character::CanActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation)
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, bAllowRemoteActivation);
	}

	return false;
}



