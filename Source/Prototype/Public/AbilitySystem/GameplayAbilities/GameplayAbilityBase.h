// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilityBase.generated.h"

UENUM(BlueprintType)
enum class EPrototypeAbilityActivationPolicy: uint8
{
	// Try to activate the ability when the input is triggered.
	OnInputTriggered,

	// Continually try to activate the ability while the input is active.
	WhileInputActive,

	// Try to activate the ability when an avatar is assigned.
	OnSpawn
};
/**
 * 
 */
UCLASS()
class PROTOTYPE_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UGameplayAbilityBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag AbilityTag;
	EPrototypeAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Prototype |Ability Activation")
	EPrototypeAbilityActivationPolicy ActivationPolicy;
	void TryActivateAbilityOnSpawn(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) const;
	
};
