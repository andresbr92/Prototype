// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/PTInventoryManagerComponent.h"


#include "Engine/ActorChannel.h"
#include "Engine/World.h"
// #include "GameFramework/GameplayMessageSubsystem.h"
#include "../Public/Inventory/PTInventoryItemDefinition.h"
#include "../Public/Inventory/PTInventoryItemInstance.h"
#include "NativeGameplayTags.h"
#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
#include UE_INLINE_GENERATED_CPP_BY_NAME(PTInventoryManagerComponent)


//////////////////////////////////////////////////////////////////////
// FLyraInventoryEntry

FString FPTInventoryEntry::GetDebugString() const
{
	TSubclassOf<UPTInventoryItemDefinition> ItemDef;
	if (Instance != nullptr)
	{
		// ItemDef = Instance->GetItemDef();
	}

	return FString::Printf(TEXT("%s (%d x %s)"), *GetNameSafe(Instance), StackCount, *GetNameSafe(ItemDef));
}

//////////////////////////////////////////////////////////////////////
// FPTInventoryList

void FPTInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		FPTInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.StackCount, /*NewCount=*/ 0);
		Stack.LastObservedCount = 0;
	}
}

void FPTInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		FPTInventoryEntry& Stack = Entries[Index];
		BroadcastChangeMessage(Stack, /*OldCount=*/ 0, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FPTInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		FPTInventoryEntry& Stack = Entries[Index];
		check(Stack.LastObservedCount != INDEX_NONE);
		BroadcastChangeMessage(Stack, /*OldCount=*/ Stack.LastObservedCount, /*NewCount=*/ Stack.StackCount);
		Stack.LastObservedCount = Stack.StackCount;
	}
}

void FPTInventoryList::BroadcastChangeMessage(FPTInventoryEntry& Entry, int32 OldCount, int32 NewCount)
{
	// FLyraInventoryChangeMessage Message;
	// Message.InventoryOwner = OwnerComponent;
	// Message.Instance = Entry.Instance;
	// Message.NewCount = NewCount;
	// Message.Delta = NewCount - OldCount;
	//
	// UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(OwnerComponent->GetWorld());
	// MessageSystem.BroadcastMessage(TAG_Lyra_Inventory_Message_StackChanged, Message);
}

UPTInventoryItemInstance* FPTInventoryList::AddEntry(TSubclassOf<UPTInventoryItemDefinition> ItemDef, int32 StackCount)
{
	UPTInventoryItemInstance* Result = nullptr;

	check(ItemDef != nullptr);
 	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());


	FPTInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UPTInventoryItemInstance>(OwnerComponent->GetOwner());  //@TODO: Using the actor instead of component as the outer due to UE-127172
	NewEntry.Instance->SetItemDef(ItemDef);
	for (UPTInventoryItemFragment* Fragment : GetDefault<UPTInventoryItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	NewEntry.StackCount = StackCount;
	Result = NewEntry.Instance;
	
	//const UPTInventoryItemDefinition* ItemCDO = GetDefault<UPTInventoryItemDefinition>(ItemDef);
	MarkItemDirty(NewEntry);

	return Result;
}

void FPTInventoryList::AddEntry(UPTInventoryItemInstance* Instance)
{
	unimplemented();
}

void FPTInventoryList::RemoveEntry(UPTInventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FPTInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

TArray<UPTInventoryItemInstance*> FPTInventoryList::GetAllItems() const
{
	TArray<UPTInventoryItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FPTInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr) //@TODO: Would prefer to not deal with this here and hide it further?
		{
			Results.Add(Entry.Instance);
		}
	}
	return Results;
}

//////////////////////////////////////////////////////////////////////
// UPTInventoryManagerComponent

UPTInventoryManagerComponent::UPTInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
	SetIsReplicatedByDefault(true);
}

void UPTInventoryManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

bool UPTInventoryManagerComponent::CanAddItemDefinition(TSubclassOf<UPTInventoryItemDefinition> ItemDef, int32 StackCount)
{
	//@TODO: Add support for stack limit / uniqueness checks / etc...
	return true;
}

UPTInventoryItemInstance* UPTInventoryManagerComponent::AddItemDefinition(TSubclassOf<UPTInventoryItemDefinition> ItemDef, int32 StackCount)
{
	UPTInventoryItemInstance* Result = nullptr;
	if (ItemDef != nullptr)
	{
		Result = InventoryList.AddEntry(ItemDef, StackCount);
		
		if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && Result)
		{
			AddReplicatedSubObject(Result);
		}
	}
	return Result;
}

void UPTInventoryManagerComponent::AddItemInstance(UPTInventoryItemInstance* ItemInstance)
{
	InventoryList.AddEntry(ItemInstance);
	if (IsUsingRegisteredSubObjectList() && IsReadyForReplication() && ItemInstance)
	{
		AddReplicatedSubObject(ItemInstance);
	}
}

void UPTInventoryManagerComponent::RemoveItemInstance(UPTInventoryItemInstance* ItemInstance)
{
	InventoryList.RemoveEntry(ItemInstance);

	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}

TArray<UPTInventoryItemInstance*> UPTInventoryManagerComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

UPTInventoryItemInstance* UPTInventoryManagerComponent::FindFirstItemStackByDefinition(TSubclassOf<UPTInventoryItemDefinition> ItemDef) const
{
	for (const FPTInventoryEntry& Entry : InventoryList.Entries)
	{
		UPTInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			// if (Instance->GetItemDef() == ItemDef)
			// {
			// 	return Instance;
			// }
		}
	}

	return nullptr;
}

int32 UPTInventoryManagerComponent::GetTotalItemCountByDefinition(TSubclassOf<UPTInventoryItemDefinition> ItemDef) const
{
	int32 TotalCount = 0;
	for (const FPTInventoryEntry& Entry : InventoryList.Entries)
	{
		UPTInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			// if (Instance->GetItemDef() == ItemDef)
			// {
			// 	++TotalCount;
			// }
		}
	}

	return TotalCount;
}

bool UPTInventoryManagerComponent::ConsumeItemsByDefinition(TSubclassOf<UPTInventoryItemDefinition> ItemDef, int32 NumToConsume)
{
	AActor* OwningActor = GetOwner();
	if (!OwningActor || !OwningActor->HasAuthority())
	{
		return false;
	}

	//@TODO: N squared right now as there's no acceleration structure
	int32 TotalConsumed = 0;
	while (TotalConsumed < NumToConsume)
	{
		if (UPTInventoryItemInstance* Instance = UPTInventoryManagerComponent::FindFirstItemStackByDefinition(ItemDef))
		{
			InventoryList.RemoveEntry(Instance);
			++TotalConsumed;
		}
		else
		{
			return false;
		}
	}

	return TotalConsumed == NumToConsume;
}

void UPTInventoryManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing UPTInventoryItemInstance
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FPTInventoryEntry& Entry : InventoryList.Entries)
		{
			UPTInventoryItemInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

bool UPTInventoryManagerComponent::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FPTInventoryEntry& Entry : InventoryList.Entries)
	{
		UPTInventoryItemInstance* Instance = Entry.Instance;

		if (Instance && IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}