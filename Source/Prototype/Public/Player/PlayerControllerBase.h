// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerBase.generated.h"

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
	APlayerStateBase* GetPlayerStateBase() const;
public:
	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TObjectPtr<UPTInventoryManagerComponent> InventoryManagerComponent;
	
	virtual void PreProcessInput(const float DeltaTime, const bool bGamePaused) override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	
};
