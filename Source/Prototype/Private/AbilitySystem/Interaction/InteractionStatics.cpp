// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Interaction/InteractionStatics.h"

#include "AbilitySystem/Interaction/IInteractableTarget.h"

UInteractionStatics::UInteractionStatics()
	: Super(FObjectInitializer::Get())
{
}

AActor* UInteractionStatics::GetActorFromInteractableTarget(TScriptInterface<IInteractableTarget> InteractableTarget)
{
	if (UObject* Object = InteractableTarget.GetObject())
	{
		if (AActor* Actor = Cast<AActor>(Object))
		{
			return Actor;
		} else if (UActorComponent* ActorComponent = Cast<UActorComponent>(Object))
		{
			AActor* ActorToReturn = ActorComponent->GetOwner();
			return ActorToReturn;
		} else
		{
			unimplemented()
		}
	}
	return nullptr;
}

void UInteractionStatics::GetInteractableTargetsFromActor(AActor* Actor,
	TArray<TScriptInterface<IInteractableTarget>>& OutInteractableTargets)
{
	// If the actor is directly interactable, return that.
	TScriptInterface<IInteractableTarget> InteractableActor(Actor);
	if (InteractableActor)
	{
		OutInteractableTargets.Add(InteractableActor);
	}

	// If the actor isn't interactable, it might have a component that has a interactable interface.
	TArray<UActorComponent*> InteractableComponents = Actor ? Actor->GetComponentsByInterface(UInteractableTarget::StaticClass()) : TArray<UActorComponent*>();
	for (UActorComponent* InteractableComponent : InteractableComponents)
	{
		OutInteractableTargets.Add(TScriptInterface<IInteractableTarget>(InteractableComponent));
	}
}
