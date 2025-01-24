// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/PrototypeEquipmentManagerComponent.h"

#include "AbilitySystemGlobals.h"
#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"
#include "Engine/ActorChannel.h"
#include "Equipment/PrototypeEquipmentDefinition.h"
#include "Equipment/PrototypeEquipmentInstance.h"
#include "Net/UnrealNetwork.h"

class FLifetimeProperty;
struct FReplicationFlags;

FString FPTAppliedEquipmentEntry::GetDebugString() const
{
	return FString::Printf(TEXT("%s of %s"), *GetNameSafe(Instance), *GetNameSafe(EquipmentDefinition.Get()));
	
}

//////////////////////////////////////////////////////////////////////
// FLyraEquipmentList

void FPTEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
 	for (int32 Index : RemovedIndices)
 	{
 		const FPTAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnUnequipped();
		}
 	}
}

void FPTEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FPTAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnEquipped();
		}
	}
}

void FPTEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
// 	for (int32 Index : ChangedIndices)
// 	{
// 		const FGameplayTagStack& Stack = Stacks[Index];
// 		TagToCountMap[Stack.Tag] = Stack.StackCount;
// 	}
}

UCustomAbilitySystemComponent* FPTEquipmentList::GetCustomAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	return Cast<UCustomAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}
UPrototypeEquipmentInstance* UPrototypeEquipmentManagerComponent::EquipItem(TSubclassOf<UPrototypeEquipmentDefinition> EquipmentClass)
{
	UPrototypeEquipmentInstance* Result = nullptr;
	if (EquipmentClass != nullptr)
	{
		Result = EquipmentList.AddEntry(EquipmentClass);
		if (Result != nullptr)
		{
			Result->OnEquipped();

			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(Result);
			}
		}
	}
	return Result;
}

UPrototypeEquipmentInstance* FPTEquipmentList::AddEntry(TSubclassOf<UPrototypeEquipmentDefinition> EquipmentDefinition)
{
	UPrototypeEquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
 	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());
	// 	LogTemp:   EquipmentDefinition: ED_Sword_C
	// LogTemp:   OwnerComponent: PrototypeEquipmentManager
	// LogTemp:   OwnerComponent's Owner: BP_PrototypeCharacter_C_0
	// LogTemp:   NewEntry.Instance (Newly Created): PrototypeEquipmentInstance_0
	UE_LOG(LogTemp, Log, TEXT("  EquipmentDefinition: %s"), *GetNameSafe(EquipmentDefinition));
	UE_LOG(LogTemp, Log, TEXT("  OwnerComponent: %s"), *GetNameSafe(OwnerComponent));
	UE_LOG(LogTemp, Log, TEXT("  OwnerComponent's Owner: %s"), *GetNameSafe(OwnerComponent->GetOwner()));
	
	const UPrototypeEquipmentDefinition* EquipmentCDO = GetDefault<UPrototypeEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UPrototypeEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = UPrototypeEquipmentInstance::StaticClass();
	}

	
	FPTAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UPrototypeEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType); //@TODO: Using the actor instead of component as the outer due to UE-127172
	Result = NewEntry.Instance;
	UE_LOG(LogTemp, Log, TEXT("  NewEntry.Instance (Newly Created): %s"), *GetNameSafe(NewEntry.Instance));

	if (UCustomAbilitySystemComponent* ASC = GetCustomAbilitySystemComponent())
	{
		for (const TObjectPtr<const UPrototypeAbilitySet>& AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, /*inout*/ &NewEntry.GrantedHandles, Result);
		}
	}
	else
	{
		//@TODO: Warning logging?
	}

	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);


	MarkItemDirty(NewEntry);

	return Result;
}

void FPTEquipmentList::RemoveEntry(UPrototypeEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FPTAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			if (UCustomAbilitySystemComponent* ASC = GetCustomAbilitySystemComponent())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			Instance->DestroyEquipmentActors();
			

			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

//////////////////////////////////////////////////////////////////////
// ULyraEquipmentManagerComponent

UPrototypeEquipmentManagerComponent::UPrototypeEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void UPrototypeEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}



void UPrototypeEquipmentManagerComponent::UnequipItem(UPrototypeEquipmentInstance* ItemInstance)
{
	if (ItemInstance != nullptr)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(ItemInstance);
		}

		ItemInstance->OnUnequipped();
		EquipmentList.RemoveEntry(ItemInstance);
	}
}

bool UPrototypeEquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FPTAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		UPrototypeEquipmentInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void UPrototypeEquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UPrototypeEquipmentManagerComponent::UninitializeComponent()
{
	TArray<UPrototypeEquipmentInstance*> AllEquipmentInstances;

	// gathering all instances before removal to avoid side effects affecting the equipment list iterator	
	for (const FPTAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		AllEquipmentInstances.Add(Entry.Instance);
	}

	for (UPrototypeEquipmentInstance* EquipInstance : AllEquipmentInstances)
	{
		UnequipItem(EquipInstance);
	}

	Super::UninitializeComponent();
}

void UPrototypeEquipmentManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing LyraEquipmentInstances
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FPTAppliedEquipmentEntry& Entry : EquipmentList.Entries)
		{
			UPrototypeEquipmentInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

UPrototypeEquipmentInstance* UPrototypeEquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<UPrototypeEquipmentInstance> InstanceType)
{
	for (FPTAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UPrototypeEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

TArray<UPrototypeEquipmentInstance*> UPrototypeEquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<UPrototypeEquipmentInstance> InstanceType) const
{
	TArray<UPrototypeEquipmentInstance*> Results;
	for (const FPTAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UPrototypeEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}


