// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"



#include "Templates/SubclassOf.h"
#include "PTInventoryItemInstance.generated.h"

class FLifetimeProperty;
class UPTInventoryItemFragment;
class UPTInventoryItemDefinition;
/**
 * 
 */
// class FLifetimeProperty;
// class UPTInventoryItemFragment;
// struct FFrame;
// struct FGameplayTag;

UCLASS()
class PROTOTYPE_API UPTInventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	UPTInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UObject interface
	virtual bool IsSupportedForNetworking() const override { return true; }
	//~End of UObject interface
	
	// // Adds a specified number of stacks to the tag (does nothing if StackCount is below 1)
	// UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Inventory)
	// void AddStatTagStack(FGameplayTag Tag, int32 StackCount);
	//
	// // Removes a specified number of stacks from the tag (does nothing if StackCount is below 1)
	// UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category= Inventory)
	// void RemoveStatTagStack(FGameplayTag Tag, int32 StackCount);
	//
	// UFUNCTION(BlueprintCallable, Category=Inventory)
	// int32 GetStatTagStackCount(FGameplayTag Tag) const;
	//
	// // Returns true if there is at least one stack of the specified tag
	// UFUNCTION(BlueprintCallable, Category=Inventory)
	// bool HasStatTag(FGameplayTag Tag) const;

	TSubclassOf<UPTInventoryItemDefinition> GetItemDef() const
	{
		return ItemDef;
	}
	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(DeterminesOutputType=FragmentClass))
	const UPTInventoryItemFragment* FindFragmentByClass(TSubclassOf<UPTInventoryItemFragment> FragmentClass) const;
	
	template <typename ResultClass>
const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}
private:
#if UE_WITH_IRIS
	/** Register all replication fragments */
	virtual void RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context, UE::Net::EFragmentRegistrationFlags RegistrationFlags) override;
#endif // UE_WITH_IRIS
	void SetItemDef(TSubclassOf<UPTInventoryItemDefinition> InDef);
	
	friend struct FPTInventoryList;
private:
	// UPROPERTY(Replicated)
	// FGameplayTagStackContainer StatTags;

	// The item definition
	UPROPERTY(Replicated)
	TSubclassOf<UPTInventoryItemDefinition> ItemDef;
	
	
};
