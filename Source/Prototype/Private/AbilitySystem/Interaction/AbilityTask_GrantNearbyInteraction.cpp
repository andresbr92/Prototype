// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Interaction/AbilityTask_GrantNearbyInteraction.h"

#include "AbilitySystemComponent.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "GameFramework/Controller.h"

#include "TimerManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AbilityTask_GrantNearbyInteraction)
UAbilityTask_GrantNearbyInteraction::UAbilityTask_GrantNearbyInteraction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}
void UAbilityTask_GrantNearbyInteraction::Activate()
{
	Super::Activate();
}

UAbilityTask_GrantNearbyInteraction* UAbilityTask_GrantNearbyInteraction::GrantAbilitiesForNearbyInteractors(UGameplayAbility* OwningAbility,
	float InteractionScanRange, float InteractionScanRate)
{
	UAbilityTask_GrantNearbyInteraction* MyObj = NewAbilityTask<UAbilityTask_GrantNearbyInteraction>(OwningAbility);
	MyObj->InteractionScanRange = InteractionScanRange;
	MyObj->InteractionScanRate = InteractionScanRate;
	return MyObj;
}

void UAbilityTask_GrantNearbyInteraction::OnDestroy(bool AbilityEnded)
{
	Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_GrantNearbyInteraction::QueryInteractables()
{
}
