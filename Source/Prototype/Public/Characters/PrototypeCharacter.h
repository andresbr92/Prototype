// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PrototypeCharacter.generated.h"

class UPrototypeEquipmentManagerComponent;
class UPrototypeQuickBarComponent;
class UPTInventoryManagerComponent;
class UPrototypeAbilitySet;
class AItemBase;
class UInputActionToGameplayTag;
class APlayerStateBase;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

struct FInputActionValue;


UCLASS(config=Game)
class APrototypeCharacter : public ACharacterBase 
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

public:
	APrototypeCharacter(const class FObjectInitializer& ObjectInitializer);
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);



	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void InitializeAbilitySystem();
	virtual void InitializeAttributes() override;
	

	


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPrototypeEquipmentManagerComponent> EquipmentManager;
	virtual void AddCharacterAbilities() override;


	
private:
	
	UPROPERTY(EditDefaultsOnly, Category= "Input")
	TObjectPtr<UInputActionToGameplayTag> InputActionToGameplayTag;
	

	void AbilityInputTagPressed(FGameplayTag GameplayTag);
	void AbilityInputTagReleased(FGameplayTag GameplayTag);
	void AbilityInputTagHeld(FGameplayTag GameplayTag);
	
	
};

