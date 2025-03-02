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
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	


	void EquipAbility();
	void GrantInteractionAbility(const TSubclassOf<UGameplayAbilityBase>& InteractionAbility);
	void ClearInteractionAbility();
	void TryActivateAbilityOnSpawn();
protected:
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;
	
	


private:
	FGameplayAbilitySpecHandle InteractionAbilityHandle;
public:
	bool bStartupEffectsApplied = false;
	bool bCharacterAbilitiesGiven = false;
	
	// Handles to abilities that had their input pressed this frame.
	UPROPERTY(VisibleAnywhere, Category = "Ability System")
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;

	// Handles to abilities that had their input released this frame.
	UPROPERTY(VisibleAnywhere, Category = "Ability System")
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;

	
	// Handles to abilities that have their input held.
	UPROPERTY(VisibleAnywhere, Category = "Ability System")
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};
