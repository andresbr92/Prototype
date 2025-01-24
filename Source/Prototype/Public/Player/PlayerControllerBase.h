// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

class UPrototypeQuickBarComponent;
class UCustomAbilitySystemComponent;
class APlayerStateBase;
class UPTInventoryManagerComponent;
/**
 * 
 */
UCLASS()
class PROTOTYPE_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	APlayerControllerBase();
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TObjectPtr<UPTInventoryManagerComponent> InventoryManagerComponent;
	UPROPERTY(EditDefaultsOnly, Category="Equipment")
	TObjectPtr<UPrototypeQuickBarComponent> QuickBarComponent;
	
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	
	UFUNCTION(BlueprintCallable, Category ="Prototype|PlayerController")
	APlayerStateBase* GetPlayerStateBase() const;
	
	UFUNCTION(BlueprintCallable, Category="Prototype|PlayerController")
	UCustomAbilitySystemComponent* GetCustomAbilitySystemComponent() const;
	

private:
	TObjectPtr<APlayerStateBase> PlayerStateBase;
	
};
