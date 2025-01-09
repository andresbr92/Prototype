// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/GameplayAbility_Interact.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Interaction/GrantNearbyInteraction.h"

UGameplayAbility_Interact::UGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void UGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGameplayAbility_Interact::TriggerInteraction()
{
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystem && AbilitySystem->GetOwnerRole() == ROLE_Authority)
	{
		UGrantNearbyInteraction* Task = UGrantNearbyInteraction::GrantAbilitiesForNearbyInteractors(this, InteractionScanRange, InteractionScanRate);
		Task->ReadyForActivation();
	}
}
