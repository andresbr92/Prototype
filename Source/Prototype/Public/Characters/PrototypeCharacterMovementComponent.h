// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PrototypeCharacterMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROTOTYPE_API UPrototypeCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPrototypeCharacterMovementComponent();

protected:


public:
	
	virtual float GetMaxSpeed() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float SprintSpeedMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch")
	float CrouchSpeedMultiplier;
	
	uint8 RequestToStartSprinting : 1;
	uint8 RequestToStartCrouching : 1;
	// Sprint
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StartSprinting();
	UFUNCTION(BlueprintCallable, Category = "Sprint")
	void StopSprinting();

	// Crouch
	UFUNCTION(BlueprintCallable, Category = "Crouch")
	void StartCrouching();
	UFUNCTION(BlueprintCallable, Category = "Crouch")
	void StopCrouching();
};
