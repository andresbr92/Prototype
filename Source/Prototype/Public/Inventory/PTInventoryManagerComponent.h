// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "PTInventoryManagerComponent.generated.h"
class UPTInventoryItemDefinition;
class UPTInventoryItemInstance;
class UPTInventoryManagerComponent;
struct FPTInventoryList;

USTRUCT(BlueprintType)
struct FPTInventoryEntry: public FFastArraySerializerItem
{
	GENERATED_BODY()
	FPTInventoryEntry(){}
	FString GetDebugString() const;
private:
	friend FPTInventoryList;
	friend UPTInventoryManagerComponent;
	
	UPROPERTY()
	TObjectPtr<UPTInventoryItemInstance> Instance = nullptr;
	
	UPROPERTY()
	int32 StackCount = 0;
	
	UPROPERTY(NotReplicated)
	int32 LastObservedCount = INDEX_NONE;
	
};

USTRUCT(BlueprintType)
struct FPTInventoryList: public FFastArraySerializer
{
	GENERATED_BODY()
	FPTInventoryList()
	: OwnerComponent(nullptr)
	{
	}
	FPTInventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}
	
	TArray<UPTInventoryItemInstance*> GetAllItems() const;
public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FPTInventoryEntry, FPTInventoryList>(Entries, DeltaParms, *this);
	}
	/**
	 * Create a new inventory entry with the specified item definition and stack count.
	 *
	 * @param ItemClass The class of the inventory item to add.
	 * @param StackCount The number of stacks for the new inventory item.
	 * @return A pointer to the newly created inventory item instance.
	 */
	UPTInventoryItemInstance* AddEntry(TSubclassOf<UPTInventoryItemDefinition> ItemClass, int32 StackCount);
	/**
	 * This function is intended to handle adding an already existing ULyraInventoryItemInstance directly to the inventory.
	 * Not implemented yet.
	 * @param ItemInstance The inventory item instance to add.
	 */
	void AddEntry(UPTInventoryItemInstance* ItemInstance);
	
	void RemoveEntry(UPTInventoryItemInstance* ItemInstance);
	
private:
	void BroadcastChangeMessage(FPTInventoryEntry& Entry, int32 OldCount, int32 NewCount);
	
	friend UPTInventoryManagerComponent;

private:
	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
	UPROPERTY(VisibleAnywhere)
	TArray<FPTInventoryEntry> Entries;
	
	
};
template<>
struct TStructOpsTypeTraits<FPTInventoryList> : public TStructOpsTypeTraitsBase2<FPTInventoryList>
{
	enum { WithNetDeltaSerializer = true };
};
UCLASS(BlueprintType, meta=(BlueprintSpawnableComponent))
class PROTOTYPE_API UPTInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this component's properties
	UPTInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	bool CanAddItemDefinition(TSubclassOf<UPTInventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	UPTInventoryItemInstance* AddItemDefinition(TSubclassOf<UPTInventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void AddItemInstance(UPTInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	void RemoveItemInstance(UPTInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure=false)
	TArray<UPTInventoryItemInstance*> GetAllItems() const;

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure)
	UPTInventoryItemInstance* FindFirstItemStackByDefinition(TSubclassOf<UPTInventoryItemDefinition> ItemDef) const;

	int32 GetTotalItemCountByDefinition(TSubclassOf<UPTInventoryItemDefinition> ItemDef) const;
	bool ConsumeItemsByDefinition(TSubclassOf<UPTInventoryItemDefinition> ItemDef, int32 NumToConsume);

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void ReadyForReplication() override;
	//~End of UObject interface


	
private:
	UPROPERTY(Replicated, VisibleAnywhere)
	FPTInventoryList InventoryList;
	
	

};
