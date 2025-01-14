// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/IPickupable.h"
#include "Items/ItemBase.h"
#include "WeaponBase.generated.h"

class UGameplayAbilityBase;
/**
 * 
 */
UCLASS()
class PROTOTYPE_API AWeaponBase : public AItemBase, public IPickupable
{
	GENERATED_BODY()
	

public:
	AWeaponBase();
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& OptionBuilder) override;
	virtual FInventoryPickup GetPickupInventory() const override;

protected:
	

	UPROPERTY(EditAnywhere)
	FInventoryPickup StaticInventory;


	
};
