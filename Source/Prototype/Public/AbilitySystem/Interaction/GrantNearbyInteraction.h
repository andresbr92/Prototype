// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GrantNearbyInteraction.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UGrantNearbyInteraction : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

	
	virtual void Activate() override;

	/** Wait until an overlap occurs. This will need to be better fleshed out so we can specify game specific collision requirements */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UGrantNearbyInteraction* GrantAbilitiesForNearbyInteractors(UGameplayAbility* OwningAbility, float InteractionScanRange, float InteractionScanRate);

private:
	virtual void OnDestroy(bool AbilityEnded) override;

	void QueryInteractables();
	float InteractionScanRange = 100;
	float InteractionScanRate = 0.100;
	FTimerHandle QueryTimerHandle;

	TMap<FObjectKey, FGameplayAbilitySpecHandle> InteractionAbilityCache;

};
