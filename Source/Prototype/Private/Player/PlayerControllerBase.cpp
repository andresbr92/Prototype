// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerControllerBase.h"

#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"
#include "Characters/CharacterBase.h"
#include "Inventory/PTInventoryManagerComponent.h"


APlayerControllerBase::APlayerControllerBase()
{
	InventoryManagerComponent = CreateDefaultSubobject<UPTInventoryManagerComponent>("InventoryManager");
}
