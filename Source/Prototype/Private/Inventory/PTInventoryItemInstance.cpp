// Fill out your copyright notice in the Description page of Project Settings.
#include "Inventory/PTInventoryItemInstance.h"
#include "../Public/Inventory/PTInventoryItemDefinition.h"
#include "Net/UnrealNetwork.h"

#if UE_WITH_IRIS
#include "Iris/ReplicationSystem/ReplicationFragmentUtil.h"
#endif // UE_WITH_IRIS


class FLifetimeProperty;

UPTInventoryItemInstance::UPTInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}
void UPTInventoryItemInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME(ThisClass, StatTags);
	DOREPLIFETIME(ThisClass, ItemDef);
}

void UPTInventoryItemInstance::SetItemDef(TSubclassOf<UPTInventoryItemDefinition> InDef)
{
	ItemDef = InDef;
}
// #if UE_WITH_IRIS
// void UPTInventoryItemInstance::RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context, UE::Net::EFragmentRegistrationFlags RegistrationFlags)
// {
// 	using namespace UE::Net;
//
// 	// Build descriptors and allocate PropertyReplicationFragments for this object
// 	FReplicationFragmentUtil::CreateAndRegisterFragmentsForObject(this, Context, RegistrationFlags);
// }
// #endif // UE_WITH_IRIS
