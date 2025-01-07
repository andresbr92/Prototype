// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CustomAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UCustomAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	virtual void GrantDefaultAbilities(TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities);
	
	
};
