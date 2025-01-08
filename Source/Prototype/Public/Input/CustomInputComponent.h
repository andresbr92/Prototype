// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystem/Data/InputActionToGameplayTag.h"
#include "CustomInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API UCustomInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(UInputActionToGameplayTag* InputActionToGameplayTag, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);

};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UCustomInputComponent::BindAbilityActions(UInputActionToGameplayTag* InputActionToGameplayTag, UserClass* Object, PressedFuncType PressedFunc,
	ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputActionToGameplayTag)
	for (const auto AbilityWithTag: InputActionToGameplayTag->InputActionToGameplayTag)
	{
		if (AbilityWithTag.InputAction && AbilityWithTag.GameplayTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(AbilityWithTag.InputAction, ETriggerEvent::Started, Object, PressedFunc, AbilityWithTag.GameplayTag);
			}
			if (ReleasedFunc)
			{
				BindAction(AbilityWithTag.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, AbilityWithTag.GameplayTag);
			}
			if (HeldFunc)
			{
				BindAction(AbilityWithTag.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, AbilityWithTag.GameplayTag);
			}
		}
	}
}
