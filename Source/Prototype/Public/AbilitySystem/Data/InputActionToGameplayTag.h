// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InputActionToGameplayTag.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FInputActionToGameplayTagStruct
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag GameplayTag = FGameplayTag();
	
	
	
	
};
UCLASS()
class PROTOTYPE_API UInputActionToGameplayTag : public UDataAsset
{
	GENERATED_BODY()
public:
	
	const UInputAction* GetInputActionByGameplayTag(FGameplayTag GameplayTag, bool bLogNotFound = false) const;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FInputActionToGameplayTagStruct> InputActionToGameplayTag;
	
};
