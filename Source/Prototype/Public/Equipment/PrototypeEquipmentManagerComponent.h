// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/PrototypeAbilitySet.h"
#include "Components/ActorComponent.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "PrototypeEquipmentManagerComponent.generated.h"

struct FPTEquipmentList;
class UActorComponent;
class UPrototypeEquipmentInstance;
class UPrototypeEquipmentDefinition;
class UPrototypeEquipmentManagerComponent;
struct FFrame;
struct FLyraEquipmentList;
struct FNetDeltaSerializeInfo;
struct FReplicationFlags;
class UObject;

USTRUCT(BlueprintType)
struct FPTAppliedEquipmentEntry: public FFastArraySerializerItem
{
	GENERATED_BODY()

	FPTAppliedEquipmentEntry()
	{}
	FString GetDebugString() const;

private:
	friend FPTEquipmentList;
	friend UPrototypeEquipmentManagerComponent;

	// The equipment class that got equipped
	UPROPERTY()
	TSubclassOf<UPrototypeEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UPrototypeEquipmentInstance> Instance;

	// Authority-only list of grante handles
	UPROPERTY(NotReplicated)
	FPrototypeAbilitySet_GrantedHandles GrantedHandles;
	//
	
};

USTRUCT(BlueprintType)
struct FPTEquipmentList: public FFastArraySerializer
{
	GENERATED_BODY()
	FPTEquipmentList()
		: OwnerComponent(nullptr)
	{
	}
	FPTEquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}
public:
	
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract
	
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FPTAppliedEquipmentEntry, FPTEquipmentList>(Entries, DeltaParms, *this);
	}
	
	UPrototypeEquipmentInstance* AddEntry(TSubclassOf<UPrototypeEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UPrototypeEquipmentInstance* Instance);
	
private:
	
	UCustomAbilitySystemComponent* GetCustomAbilitySystemComponent() const;
	friend UPrototypeEquipmentManagerComponent;
	
private:
	UPROPERTY()
	TArray<FPTAppliedEquipmentEntry> Entries;
	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};
template<>
struct TStructOpsTypeTraits<FPTEquipmentList> : public TStructOpsTypeTraitsBase2<FPTEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};



UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROTOTYPE_API UPrototypeEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPrototypeEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UPrototypeEquipmentInstance* EquipItem(TSubclassOf<UPrototypeEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UnequipItem(UPrototypeEquipmentInstance* ItemInstance);

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface

	//~UActorComponent interface
	//virtual void EndPlay() override;
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void ReadyForReplication() override;
	//~End of UActorComponent interface

	/** Returns the first equipped instance of a given type, or nullptr if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPrototypeEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UPrototypeEquipmentInstance> InstanceType);

	/** Returns all equipped instances of a given type, or an empty array if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UPrototypeEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UPrototypeEquipmentInstance> InstanceType) const;

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

private:
	UPROPERTY(Replicated)
	FPTEquipmentList EquipmentList;

	
};
