// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/InputActionToGameplayTag.h"

const UInputAction* UInputActionToGameplayTag::GetInputActionByGameplayTag(const FGameplayTag GameplayTag, bool bLogNotFound) const
{
	for (auto OneInputActionToGameplayTag: InputActionToGameplayTag)
	{
		if (OneInputActionToGameplayTag.InputAction && GameplayTag == OneInputActionToGameplayTag.GameplayTag)
		{
			return OneInputActionToGameplayTag.InputAction;
		}
		
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find  for InputTag [%s], on InputConfig [%s]"), *GameplayTag.ToString(), *GetNameSafe(this));
	}
	return nullptr;
}
