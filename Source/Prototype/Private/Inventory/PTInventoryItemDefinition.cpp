// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/PTInventoryItemDefinition.h"

UPTInventoryItemDefinition::UPTInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
{
}

const UPTInventoryItemFragment* UPTInventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<UPTInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (UPTInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}

const UPTInventoryItemFragment* ULyraInventoryFunctionLibrary::FindItemDefinitionFragment(
	TSubclassOf<UPTInventoryItemDefinition> ItemDef, TSubclassOf<UPTInventoryItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UPTInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}


