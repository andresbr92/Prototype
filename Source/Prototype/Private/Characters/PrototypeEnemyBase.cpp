// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PrototypeEnemyBase.h"

#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/AttributeSetBase.h"
#include "Characters/PrototypeCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

APrototypeEnemyBase::APrototypeEnemyBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPrototypeCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	AbilitySystemComponent = CreateDefaultSubobject<UCustomAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	// Minimal Mode means that no GameplayEffects will replicate. They will only live on the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSetBase = CreateDefaultSubobject<UAttributeSetBase>(TEXT("AttributeSet"));
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void APrototypeEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	InitializeAttributes();
	AddStartupEffects();
	AddCharacterAbilities();
	
}

void APrototypeEnemyBase::HealthChanged(const FOnAttributeChangeData& Data)
{
}

void APrototypeEnemyBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
}

void APrototypeEnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	SetOwner(NewController);
	
}

void APrototypeEnemyBase::InitializeAttributes()
{
	Super::InitializeAttributes();
}
