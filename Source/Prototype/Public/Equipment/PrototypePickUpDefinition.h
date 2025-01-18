// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PrototypePickUpDefinition.generated.h"

class UNiagaraSystem;
class UPTInventoryItemDefinition;
// class UNiagaraSystem;
class UObject;
class USoundBase;
class UStaticMesh;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Const, Meta = (DisplayName = "Prototype Pickup Data", ShortTooltip = "Data asset used to configure a pickup."))

class PROTOTYPE_API UPrototypePickUpDefinition : public UDataAsset
{
	GENERATED_BODY()
public:
	//Defines the pickup's actors to spawn, abilities to grant, and tags to add
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Prototype|Pickup|Equipment")
	TSubclassOf<UPTInventoryItemDefinition> InventoryItemDefinition;
	//Visual representation of the pickup
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Prototype|Pickup|Mesh")
	TObjectPtr<UStaticMesh> DisplayMesh;

	//Cool down time between pickups in seconds
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Prototype|Pickup")
	int32 SpawnCoolDownSeconds;

	//Sound to play when picked up
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Prototype|Pickup")
	TObjectPtr<USoundBase> PickedUpSound;

	//Sound to play when pickup is respawned
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Prototype|Pickup")
	TObjectPtr<USoundBase> RespawnedSound;

	// //Particle FX to play when picked up
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Prototype|Pickup")
	// TObjectPtr<UNiagaraSystem> PickedUpEffect;
	//
	// //Particle FX to play when pickup is respawned
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Prototype|Pickup")
	// TObjectPtr<UNiagaraSystem> RespawnedEffect;
};


