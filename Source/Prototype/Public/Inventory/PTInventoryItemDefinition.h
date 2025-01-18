// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PTInventoryItemDefinition.generated.h"

/**
 * 
 */
class UPTInventoryItemInstance;
struct FFrame;
// DefaultToInstance: new instance of that class is created every time it's used, instead of all references pointing to the same object
// Represents a fragment of an item definition
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class PROTOTYPE_API UPTInventoryItemFragment : public UObject
{
	GENERATED_BODY()
public:
	virtual void OnInstanceCreated(UPTInventoryItemInstance* Instance) const {};

	
	
};

/**
 * UPTInventoryItemDefinition
 */
UCLASS(Blueprintable, Const, Abstract)
class UPTInventoryItemDefinition : public UObject
{
	
	GENERATED_BODY()
	
public:

	UPTInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display)
	FText DisplayName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display, Instanced)
	TArray<TObjectPtr<UPTInventoryItemFragment>> Fragments;

public:
	const UPTInventoryItemFragment* FindFragmentByClass(TSubclassOf<UPTInventoryItemFragment> FragmentClass) const;
	
};


UCLASS()
class ULyraInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType=FragmentClass))
	static const UPTInventoryItemFragment* FindItemDefinitionFragment(TSubclassOf<UPTInventoryItemDefinition> ItemDef, TSubclassOf<UPTInventoryItemFragment> FragmentClass);
};
