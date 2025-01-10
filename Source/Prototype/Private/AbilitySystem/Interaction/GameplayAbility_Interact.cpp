// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Interaction/GameplayAbility_Interact.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"
#include "AbilitySystem/Interaction/AbilityTask_GrantNearbyInteraction.h"

UGameplayAbility_Interact::UGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationPolicy = EPrototypeAbilityActivationPolicy::OnSpawn;
	
}

void UGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	UCustomAbilitySystemComponent* CustomAbilitySystemComponent = Cast<UCustomAbilitySystemComponent>(AbilitySystem);
	if (CustomAbilitySystemComponent && CustomAbilitySystemComponent->GetOwnerRole() == ROLE_Authority)
	{
		UAbilityTask_GrantNearbyInteraction* Task = UAbilityTask_GrantNearbyInteraction::GrantAbilitiesForNearbyInteractors(this, InteractionScanRange, InteractionScanRate);
		Task->ReadyForActivation();
	}
}

void UGameplayAbility_Interact::TriggerInteraction()
{
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystem && AbilitySystem->GetOwnerRole() == ROLE_Authority)
	{
		UAbilityTask_GrantNearbyInteraction* Task = UAbilityTask_GrantNearbyInteraction::GrantAbilitiesForNearbyInteractors(this, InteractionScanRange, InteractionScanRate);
		Task->ReadyForActivation();
	}
}
