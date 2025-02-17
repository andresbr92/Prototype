// Fill out your copyright notice in the Description page of Project Settings.

// #include "Characters/PrototypeCharacter.h"
#include "Characters/PrototypeCharacterMovementComponent.h"

#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"
#include "Characters/PrototypeCharacter.h"


// Sets default values for this component's properties
UPrototypeCharacterMovementComponent::UPrototypeCharacterMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPrototypeCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPrototypeCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UPrototypeCharacterMovementComponent::GetMaxSpeed() const
{
	// return Super::GetMaxSpeed();
	APrototypeCharacter* Owner = Cast<APrototypeCharacter>(GetOwner());

	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Super::GetMaxSpeed();
	}

	if (Owner->GetCustomAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Character.Status.Blocked"))))
	{
		return 0.0f;
	}
	return Owner->GetCharacterMovement()->MaxWalkSpeed;
}

