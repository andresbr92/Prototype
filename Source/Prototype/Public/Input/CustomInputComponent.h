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
	for (const auto ActionToGameplayTagStruct: InputActionToGameplayTag->InputActionToGameplayTag)
	{
		if (ActionToGameplayTagStruct.InputAction && ActionToGameplayTagStruct.GameplayTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(ActionToGameplayTagStruct.InputAction, ETriggerEvent::Started, Object, PressedFunc, ActionToGameplayTagStruct.GameplayTag);
			}
			if (ReleasedFunc)
			{
				BindAction(ActionToGameplayTagStruct.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, ActionToGameplayTagStruct.GameplayTag);
			}
			if (HeldFunc)
			{
				BindAction(ActionToGameplayTagStruct.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, ActionToGameplayTagStruct.GameplayTag);
			}
		}
	}
}
