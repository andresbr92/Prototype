// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PTInventoryItemDefinition.h"
#include "PTInventoryFragment_EquippableItem.generated.h"


class UPrototypeEquipmentDefinition;
/**
 * 
 */
UCLASS()
class PROTOTYPE_API UPTInventoryFragment_EquippableItem : public UPTInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Prototype)
	TSubclassOf<UPrototypeEquipmentDefinition> EquipmentDefinition;
};
