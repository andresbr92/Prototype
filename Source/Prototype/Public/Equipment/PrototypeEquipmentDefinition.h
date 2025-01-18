// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PrototypeEquipmentDefinition.generated.h"

class UPrototypeAbilitySet;
class UPrototypeEquipmentInstance;
/**
 * FPTEquipmentDefinition is a structure that defines the properties required for
 * equipment specification in a game or simulation environment. This includes
 * the type of actor to spawn, the socket to attach, and the transformation
 * for attachment.
 */
USTRUCT()
struct FPTEquipmentActorToSpawn
{
	GENERATED_BODY()
	/**
	 * Default constructor for FPTEquipmentDefinition.
	 * Initializes an instance of the equipment definition structure.
	 */
	FPTEquipmentActorToSpawn(){}
	/**
	 * Specifies the class of actor to spawn for this equipment.
	 * This property can be edited in the editor and is categorized under "Equipment".
	 */
	UPROPERTY(EditAnywhere, Category = "Equipment")
	TSubclassOf<AActor> ActorToSpawn;

	/**
	 * The name of the socket to which the equipment will be attached.
	 * This property determines the specific attachment point on the actor.
	 */
	UPROPERTY(EditAnywhere, Category = "Equipment")
	FName AttachSocket;

	/**
	 * Specifies the transform properties for attaching equipment to a socket.
	 * This includes location, rotation, and scale relative to the target socket.
	 * It is editable in the editor and belongs to the "Equipment" category.
	 */
	UPROPERTY(EditAnywhere, Category = "Equipment")
	FTransform AttachTransform;
	
};
/**
 * UPrototypeEquipmentDefinition
 *
 * Definition of a piece of equipment that can be applied to a pawn
 */

UCLASS(Blueprintable, Const, Abstract, BlueprintType)
class PROTOTYPE_API UPrototypeEquipmentDefinition : public UObject
{
	GENERATED_BODY()
public:
	UPrototypeEquipmentDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(editdefaultsonly, Category = "Equipment")
	TSubclassOf<UPrototypeEquipmentInstance> InstanceType;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TArray<TObjectPtr<const UPrototypeAbilitySet>> AbilitySetsToGrant;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TArray<FPTEquipmentActorToSpawn> ActorsToSpawn;
};






























