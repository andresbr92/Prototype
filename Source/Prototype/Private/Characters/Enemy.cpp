// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy.h"

#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/AttributeSetBase.h"

AEnemy::AEnemy()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCustomAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AttributeSet = CreateDefaultSubobject<UAttributeSetBase>(TEXT("AttributeSet"));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	InitializeAttributes();
	
}

void AEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		
		// print who is the owner of the ability system component
		UE_LOG(LogTemp, Warning, TEXT("Owner of the enemy Ability System Component: %s"), *AbilitySystemComponent->GetOwner()->GetName());
		// print who is the avatar of the ability system component
		UE_LOG(LogTemp, Warning, TEXT("Avatar Actor of the enemy Ability System Component: %s"), *AbilitySystemComponent->GetAvatarActor()->GetName());
	}
	SetOwner(NewController);
	
}

void AEnemy::InitializeAttributes()
{
	Super::InitializeAttributes();
}
