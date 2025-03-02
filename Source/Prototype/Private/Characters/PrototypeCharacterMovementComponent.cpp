// Fill out your copyright notice in the Description page of Project Settings.

// #include "Characters/PrototypeCharacter.h"
#include "Characters/PrototypeCharacterMovementComponent.h"

#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"
#include "Characters/PrototypeCharacter.h"


// Sets default values for this component's properties
UPrototypeCharacterMovementComponent::UPrototypeCharacterMovementComponent()
{
	SprintSpeedMultiplier = 1.4f;
	CrouchSpeedMultiplier = 0.5f;
}


float UPrototypeCharacterMovementComponent::GetMaxSpeed() const
{
	// return Super::GetMaxSpeed();
	ACharacterBase* Owner = Cast<ACharacterBase>(GetOwner());

	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Super::GetMaxSpeed();
	}

	if (Owner->GetCustomAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Status.Blocked"))))
	{
		return 0.0f;
	}

	if (RequestToStartSprinting)
	{
		 return Owner->GetMoveSpeed() * SprintSpeedMultiplier;
		
	}
	if (RequestToStartCrouching)
	{
		return Owner->GetMoveSpeed() * CrouchSpeedMultiplier;
	}
	return Owner->GetMoveSpeed();
}

void UPrototypeCharacterMovementComponent::StartSprinting()
{
	RequestToStartSprinting = true;	
}

void UPrototypeCharacterMovementComponent::StopSprinting()
{
	RequestToStartSprinting = false;
}

void UPrototypeCharacterMovementComponent::StartCrouching()
{
	RequestToStartCrouching = true;
}


void UPrototypeCharacterMovementComponent::StopCrouching()
{
	RequestToStartCrouching = false;
}
