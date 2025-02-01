// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipment/PrototypeQuickBarComponent.h"

#include "Equipment/PrototypeEquipmentDefinition.h"
#include "Equipment/PrototypeEquipmentInstance.h"
#include "Equipment/PrototypeEquipmentManagerComponent.h"
// #include "GameFramework/GameplayMessageSubsystem.h"
#include "GameFramework/Pawn.h"
#include "Inventory/PTInventoryFragment_EquippableItem.h"
#include "NativeGameplayTags.h"
#include "Net/UnrealNetwork.h"

class FLifetimeProperty;
class UPrototypeEquipmentDefinition;

UPrototypeQuickBarComponent::UPrototypeQuickBarComponent(const FObjectInitializer &ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
	if (Slots.Num() < NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}
}

void UPrototypeQuickBarComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Slots);
	DOREPLIFETIME(ThisClass, ActiveSlotIndex);
}

void UPrototypeQuickBarComponent::BeginPlay()
{

	Super::BeginPlay();
}

void UPrototypeQuickBarComponent::CycleActiveSlotForward()
{
	if (Slots.Num() < 2)
	{
		return;
	}

	const int32 OldIndex = (ActiveSlotIndex < 0 ? Slots.Num() - 1 : ActiveSlotIndex);
	int32 NewIndex = ActiveSlotIndex;
	do
	{
		NewIndex = (NewIndex + 1) % Slots.Num();
		if (Slots[NewIndex] != nullptr)
		{
			SetActiveSlotIndex(NewIndex);
			return;
		}
	} while (NewIndex != OldIndex);
}

void UPrototypeQuickBarComponent::CycleActiveSlotBackward()
{
	if (Slots.Num() < 2)
	{
		return;
	}

	const int32 OldIndex = (ActiveSlotIndex < 0 ? Slots.Num() - 1 : ActiveSlotIndex);
	int32 NewIndex = ActiveSlotIndex;
	do
	{
		NewIndex = (NewIndex - 1 + Slots.Num()) % Slots.Num();
		if (Slots[NewIndex] != nullptr)
		{
			SetActiveSlotIndex(NewIndex);
			return;
		}
	} while (NewIndex != OldIndex);
}

void UPrototypeQuickBarComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(EquippedItem == nullptr);

	if (UPTInventoryItemInstance *SlotItem = Slots[ActiveSlotIndex])
	{
		if (const UPTInventoryFragment_EquippableItem *EquipInfo = SlotItem->FindFragmentByClass<UPTInventoryFragment_EquippableItem>())
		{
			TSubclassOf<UPrototypeEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
			if (EquipDef != nullptr)
			{
				if (UPrototypeEquipmentManagerComponent *EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);
					if (EquippedItem != nullptr)
					{
						EquippedItem->SetInstigator(SlotItem);
					}
				}
			}
		}
	}
}

void UPrototypeQuickBarComponent::UnequipItemInSlot()
{
	if (UPrototypeEquipmentManagerComponent *EquipmentManager = FindEquipmentManager())
	{
		if (EquippedItem != nullptr)
		{
			EquipmentManager->UnequipItem(EquippedItem);
			EquippedItem = nullptr;
		}
	}
}

UPrototypeEquipmentManagerComponent *UPrototypeQuickBarComponent::FindEquipmentManager() const
{
	if (AController *OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn *Pawn = OwnerController->GetPawn())
		{
			return Pawn->FindComponentByClass<UPrototypeEquipmentManagerComponent>();
		}
	}
	return nullptr;
}

void UPrototypeQuickBarComponent::SetActiveSlotIndex_Implementation(int32 NewIndex)
{

	if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
	{
		UnequipItemInSlot();

		ActiveSlotIndex = NewIndex;

		EquipItemInSlot();

		OnRep_ActiveSlotIndex();
	}
}

UPTInventoryItemInstance *UPrototypeQuickBarComponent::GetActiveSlotItem() const
{
	return Slots.IsValidIndex(ActiveSlotIndex) ? Slots[ActiveSlotIndex] : nullptr;
}

int32 UPrototypeQuickBarComponent::GetNextFreeItemSlot() const
{
	int32 SlotIndex = 0;
	for (const TObjectPtr<UPTInventoryItemInstance> &ItemPtr : Slots)
	{
		if (ItemPtr == nullptr)
		{
			return SlotIndex;
		}
		++SlotIndex;
	}

	return INDEX_NONE;
}

void UPrototypeQuickBarComponent::AddItemToSlot(int32 SlotIndex, UPTInventoryItemInstance *Item)
{
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		if (Slots[SlotIndex] == nullptr)
		{
			Slots[SlotIndex] = Item;
			OnRep_Slots();
		}
	}
}

UPTInventoryItemInstance *UPrototypeQuickBarComponent::RemoveItemFromSlot(int32 SlotIndex)
{
	UPTInventoryItemInstance *Result = nullptr;

	if (ActiveSlotIndex == SlotIndex)
	{
		UnequipItemInSlot();
		ActiveSlotIndex = -1;
	}

	if (Slots.IsValidIndex(SlotIndex))
	{
		Result = Slots[SlotIndex];

		if (Result != nullptr)
		{
			Slots[SlotIndex] = nullptr;
			OnRep_Slots();
		}
	}

	return Result;
}

void UPrototypeQuickBarComponent::OnRep_Slots()
{
}

void UPrototypeQuickBarComponent::OnRep_ActiveSlotIndex()
{
	// print the active slot index
}
