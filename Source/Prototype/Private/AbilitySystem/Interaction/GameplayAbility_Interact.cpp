// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Interaction/GameplayAbility_Interact.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"
#include "AbilitySystem/Interaction/AbilityTask_GrantNearbyInteraction.h"

UGameplayAbility_Interact::UGameplayAbility_Interact(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationPolicy = EPrototypeAbilityActivationPolicy::OnSpawn;
}

void UGameplayAbility_Interact::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	UCustomAbilitySystemComponent* CustomAbilitySystemComponent = Cast<UCustomAbilitySystemComponent>(AbilitySystem);
	if (CustomAbilitySystemComponent && CustomAbilitySystemComponent->GetOwnerRole() == ROLE_Authority)
	{
		UAbilityTask_GrantNearbyInteraction* Task = UAbilityTask_GrantNearbyInteraction::GrantAbilitiesForNearbyInteractors(this, InteractionScanRange, InteractionScanRate);
		Task->ReadyForActivation();
	}
}

void UGameplayAbility_Interact::UpdateInteractions(const TArray<FInteractionOption>& InteractiveOptions)
{
	// Verifica si la habilidad tiene información de contexto válida
	if (CurrentActorInfo && CurrentActorInfo->PlayerController.IsValid())
	{
		// Obtén el PlayerController directamente del contexto de la habilidad
		APlayerController* PC = CurrentActorInfo->PlayerController.Get();

		// Limpia la lista de opciones actuales
		CurrentOptions = InteractiveOptions;

		// Itera sobre las opciones de interacción detectadas
		for (const FInteractionOption& InteractionOption : InteractiveOptions)
		{
			// Obtén el actor asociado a esta opción de interacción
			AActor* InteractableTargetActor = UInteractionStatics::GetActorFromInteractableTarget(InteractionOption.InteractableTarget);

			if (InteractableTargetActor)
			{
				// Muestra un mensaje con el nombre del actor interactuable
				FString ActorName = InteractableTargetActor->GetName();
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Objeto interactuable detectado: %s"), *ActorName));
			}
			else
			{
				// Mensaje de error si no se puede obtener el actor
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No se pudo obtener el actor interactuable."));
			}
		}

		// Mensaje para indicar que las opciones de interacción se actualizaron
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Opciones de interacción actualizadas."));
	}
	else
	{
		// Mensaje de advertencia si no se encuentra un PlayerController válido
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No se encontró un PlayerController válido para actualizar interacciones."));
	}
}

void UGameplayAbility_Interact::TriggerInteraction()
{
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	if (AbilitySystem && AbilitySystem->GetOwnerRole() == ROLE_Authority)
	{
		UAbilityTask_GrantNearbyInteraction* Task = UAbilityTask_GrantNearbyInteraction::GrantAbilitiesForNearbyInteractors(this, InteractionScanRange, InteractionScanRate);
		Task->ReadyForActivation();
	}
}
