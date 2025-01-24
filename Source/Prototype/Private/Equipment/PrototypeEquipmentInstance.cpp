// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/PrototypeEquipmentInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Equipment/PrototypeEquipmentDefinition.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#if UE_WITH_IRIS
#include "Iris/ReplicationSystem/ReplicationFragmentUtil.h"
#endif // UE_WITH_IRIS
class FLifetimeProperty;
class UClass;
class USceneComponent;

UPrototypeEquipmentInstance::UPrototypeEquipmentInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}
UWorld* UPrototypeEquipmentInstance::GetWorld() const
{
	if (APawn* OwningPawn = GetPawn())
	{
		return OwningPawn->GetWorld();
	}
	else
	{
		return nullptr;
	}
}
void UPrototypeEquipmentInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, Instigator);
	UE_LOG(LogTemp, Log, TEXT("ULyraEquipmentInstance::GetLifetimeReplicatedProps called! Instigator will be replicated"));
	DOREPLIFETIME(ThisClass, SpawnedActors);
	UE_LOG(LogTemp, Log, TEXT("ULyraEquipmentInstance::GetLifetimeReplicatedProps called! SpawnedActors will be replicated"));
}

#if UE_WITH_IRIS
void UPrototypeEquipmentInstance::RegisterReplicationFragments(UE::Net::FFragmentRegistrationContext& Context, UE::Net::EFragmentRegistrationFlags RegistrationFlags)
{
	using namespace UE::Net;
	UE_LOG(LogTemp, Log, TEXT("ULyraEquipmentInstance::RegisterReplicationFragments called!"));
	// Build descriptors and allocate PropertyReplicationFragments for this object
	FReplicationFragmentUtil::CreateAndRegisterFragmentsForObject(this, Context, RegistrationFlags);
}
#endif // UE_WITH_IRIS

APawn* UPrototypeEquipmentInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}
APawn* UPrototypeEquipmentInstance::GetTypedPawn(TSubclassOf<APawn> PawnType) const
{
	APawn* Result = nullptr;
	if (UClass* ActualPawnType = PawnType)
	{
		if (GetOuter()->IsA(ActualPawnType))
		{
			Result = Cast<APawn>(GetOuter());
		}
	}
	return Result;
}
void UPrototypeEquipmentInstance::SpawnEquipmentActors(const TArray<FPTEquipmentActorToSpawn>& ActorsToSpawn)
{
	if (APawn* OwningPawn = GetPawn())
	{
		USceneComponent* AttachTarget = OwningPawn->GetRootComponent();
		if (ACharacter* Char = Cast<ACharacter>(OwningPawn))
		{
			AttachTarget = Char->GetMesh();
		}

		for (const FPTEquipmentActorToSpawn& SpawnInfo : ActorsToSpawn)
		{
			AActor* NewActor = GetWorld()->SpawnActorDeferred<AActor>(SpawnInfo.ActorToSpawn, FTransform::Identity, OwningPawn);
			NewActor->FinishSpawning(FTransform::Identity, /*bIsDefaultTransform=*/ true);
			NewActor->SetActorRelativeTransform(SpawnInfo.AttachTransform);
			NewActor->AttachToComponent(AttachTarget, FAttachmentTransformRules::KeepRelativeTransform, SpawnInfo.AttachSocket);

			SpawnedActors.Add(NewActor);
			UE_LOG(LogTemp, Log, TEXT("ULyraEquipmentInstance::SpawnEquipmentActors Created Actor: %s"), *GetNameSafe(NewActor));
		}
	}
}
void UPrototypeEquipmentInstance::DestroyEquipmentActors()
{
	for (AActor* Actor : SpawnedActors)
	{
		if (Actor)
		{
			Actor->Destroy();
		}
	}
}
void UPrototypeEquipmentInstance::OnEquipped()
{
	K2_OnEquipped();
}

void UPrototypeEquipmentInstance::OnUnequipped()
{
	K2_OnUnequipped();
}

void UPrototypeEquipmentInstance::OnRep_Instigator()
{
	UE_LOG(LogTemp, Log, TEXT("ULyraEquipmentInstance::OnRep_Instigator called! Instigator Updated %s"), *GetNameSafe(Instigator));
}