// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayAbilities/GameplayAbilityBase.h"

#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"

void UGameplayAbilityBase::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	TryActivateAbilityOnSpawn(ActorInfo, Spec);
}

void UGameplayAbilityBase::TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec) const
{
	if (ActorInfo && !Spec.IsActive() && (ActivationPolicy == EPrototypeAbilityActivationPolicy::OnSpawn))
	{
		UAbilitySystemComponent* AbilitySystem = ActorInfo->AbilitySystemComponent.Get();
		AbilitySystem->TryActivateAbility(Spec.Handle);
	}
}


