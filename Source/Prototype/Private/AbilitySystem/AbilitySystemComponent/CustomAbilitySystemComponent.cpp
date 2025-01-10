// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"

#include "AbilitySystem/GameplayAbilities/GameplayAbilityBase.h"

void UCustomAbilitySystemComponent::GrantDefaultAbilities(TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities)
{
	//const TPair<FString, FString>& FileDest : FilesAndDestinations
	for ( TSubclassOf<UGameplayAbility> Ability : DefaultAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);
		
		UGameplayAbilityBase* AbilityBase = Cast<UGameplayAbilityBase>(AbilitySpec.Ability);
		
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityBase->AbilityTag);
		GiveAbility(AbilitySpec);
	}
}

void UCustomAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (auto Ability: GetActivatableAbilities())
	{
		UGameplayAbilityBase* AbilityBase = Cast<UGameplayAbilityBase>(Ability.Ability);
		if (AbilityBase->AbilityTag == InputTag)
		{
			// Try to activate the ability
			TryActivateAbility(Ability.Handle);
		}
	}
}

void UCustomAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
}

void UCustomAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
}

void UCustomAbilitySystemComponent::EquipAbility()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("EquipAbility"));
}

void UCustomAbilitySystemComponent::GrantInteractionAbility(const TSubclassOf<UGameplayAbilityBase>& InteractionAbility)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(InteractionAbility, 1);
	FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySpec.Handle;
	InteractionAbilityHandle = AbilitySpecHandle;
	
	GiveAbility(AbilitySpec);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GrantInteractionbility"));
	
}

void UCustomAbilitySystemComponent::ClearInteractionAbility()
{
	ClearAbility(InteractionAbilityHandle);
}





