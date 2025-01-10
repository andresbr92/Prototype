// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionOption.h"
#include "AbilitySystem/GameplayAbilities/GameplayAbilityBase.h"
#include "GameplayAbility_Interact.generated.h"

class UObject;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayEventData;
/**
 * 
 */
UCLASS(Abstract)
class PROTOTYPE_API UGameplayAbility_Interact : public UGameplayAbilityBase
{
	GENERATED_BODY()
	UGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void UpdateInteractions(const TArray<FInteractionOption>& InteractiveOptions);
	UFUNCTION(BlueprintCallable)
	void TriggerInteraction();

protected:

	UPROPERTY(BlueprintReadWrite)
	TArray<FInteractionOption> CurrentOptions;

	UPROPERTY(EditDefaultsOnly)
	float InteractionScanRate = 0.1f;

	UPROPERTY(EditDefaultsOnly)
	float InteractionScanRange = 500;

	// UPROPERTY(EditDefaultsOnly)
	// TSoftClassPtr<UUserWidget> DefaultInteractionWidgetClass;

	
};
