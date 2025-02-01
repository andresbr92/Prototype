// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/WeaponBase.h"

AWeaponBase::AWeaponBase()
{
	bReplicates = true;
}

void AWeaponBase::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
                                           FInteractionOptionBuilder& OptionBuilder)
{
	Super::GatherInteractionOptions(InteractQuery, OptionBuilder);
}

FInventoryPickup AWeaponBase::GetPickupInventory() const
{
	return StaticInventory;
}
