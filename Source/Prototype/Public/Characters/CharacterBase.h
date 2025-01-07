// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "CharacterBase.generated.h"

class UAttributeSetBase;
class UCustomAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class PROTOTYPE_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Implement the IAbilitySystemInterface. (To find the Ability SystemComponent)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UCustomAbilitySystemComponent* GetCustomAbilitySystemComponent() const;
	UFUNCTION(BlueprintPure, Category = "Ability System")
	UAttributeSetBase* GetAttributeSet() const { return AttributeSet; };
	
	
	

protected:
	UPROPERTY()
	TObjectPtr<UCustomAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSetBase> AttributeSet;
	
	// gameplay effect to initialize attributes
	UPROPERTY(EditDefaultsOnly, Category = "Ability System")
	TSubclassOf<UGameplayEffect> DefaultAttributeEffect;

	virtual void InitializeAttributes();
	


public:	


};
