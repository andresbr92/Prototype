// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"

void UCustomAbilitySystemComponent::GrantDefaultAbilities(TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities)
{
	//const TPair<FString, FString>& FileDest : FilesAndDestinations
	for ( TSubclassOf<UGameplayAbility> Ability : DefaultAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}
