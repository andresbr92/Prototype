// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CustomAbilitySystemComponent.generated.h"

class UGameplayAbilityBase;
/**
 * 
 */
UCLASS()
class PROTOTYPE_API UCustomAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	virtual void GrantDefaultAbilities(TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities);

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void EquipAbility();
	void GrantInteractionAbility(const TSubclassOf<UGameplayAbilityBase>& InteractionAbility);
	void ClearInteractionAbility();


private:
	FGameplayAbilitySpecHandle InteractionAbilityHandle;
	
};
