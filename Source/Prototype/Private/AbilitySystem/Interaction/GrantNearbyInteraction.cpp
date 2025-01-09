// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Interaction/GrantNearbyInteraction.h"

#include "AbilitySystemComponent.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"

#include "TimerManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GrantNearbyInteraction)
UGrantNearbyInteraction::UGrantNearbyInteraction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}
void UGrantNearbyInteraction::Activate()
{
	Super::Activate();
}

UGrantNearbyInteraction* UGrantNearbyInteraction::GrantAbilitiesForNearbyInteractors(UGameplayAbility* OwningAbility,
	float InteractionScanRange, float InteractionScanRate)
{
	UGrantNearbyInteraction* MyObj = NewAbilityTask<UGrantNearbyInteraction>(OwningAbility);
	MyObj->InteractionScanRange = InteractionScanRange;
	MyObj->InteractionScanRate = InteractionScanRate;
	return MyObj;
}

void UGrantNearbyInteraction::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);
}

void UGrantNearbyInteraction::QueryInteractables()
{
}
