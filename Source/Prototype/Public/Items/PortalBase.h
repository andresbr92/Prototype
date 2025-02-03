// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "PortalBase.generated.h"

class APrototypeCharacter;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSequenceCompleted, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInputReceived, const FGameplayTag&, ReceivedTag, bool, bWasCorrect);

UCLASS()
class PROTOTYPE_API APortalBase : public AItemBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APortalBase();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// GAS Interface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Portal Game Logic
	UFUNCTION(BlueprintCallable, Category = "Portal")
	void StartPortalSequence();

	UFUNCTION(BlueprintCallable, Category = "Portal")
	void StopPortalSequence();

	UFUNCTION(BlueprintCallable, Category = "Portal")
	void HandleInputReceived(const FGameplayTag& InputTag, APrototypeCharacter* InputPlayer);

	UFUNCTION(BlueprintCallable, Category = "Portal")
	void AddPlayerToGame(APrototypeCharacter* Player);

	UFUNCTION(BlueprintCallable, Category = "Portal")
	void RemovePlayerFromGame(APrototypeCharacter* Player);

	// Eventos
	UPROPERTY(BlueprintAssignable, Category = "Portal")
	FOnSequenceCompleted OnSequenceCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Portal")
	FOnInputReceived OnInputReceived;

protected:
	// Propiedades replicadas
	UPROPERTY(Replicated, BlueprintReadWrite, VisibleAnywhere, Category = "Portal")
	TArray<TObjectPtr<APrototypeCharacter>> CharactersPlaying;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Portal")
	TArray<FGameplayTag> TagsSequence;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Portal")
	int32 CurrentSequenceIndex;

	UPROPERTY(EditDefaultsOnly, Category = "Portal")
	TArray<TObjectPtr<UPrimitiveComponent>> CharacterCircle;

	// Configuración del portal
	UPROPERTY(EditDefaultsOnly, Category = "Portal Config")
	float TimeLimitPerInput;

	UPROPERTY(EditDefaultsOnly, Category = "Portal Config")
	int32 MaxPlayers;

	UPROPERTY(EditDefaultsOnly, Category = "Portal Config")
	TArray<TSubclassOf<UGameplayAbility>> PortalAbilities;

	// Estado del portal
	UPROPERTY(Replicated)
	bool bIsSequenceActive;

	UPROPERTY()
	FTimerHandle InputTimerHandle;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// Mapa para rastrear qué jugadores han introducido el input correcto en el paso actual
	UPROPERTY()
	TMap<TObjectPtr<APrototypeCharacter>, bool> PlayersCompletedCurrentStep;

	// Funciones protegidas
	UFUNCTION()
	void OnRep_IsSequenceActive();

	UFUNCTION()
	void HandleInputTimeout();

	UFUNCTION(Server, Reliable)
	void Server_ProcessInput(const FGameplayTag& InputTag, APrototypeCharacter* InputPlayer);

	void GrantPortalAbilitiesToPlayer(APrototypeCharacter* Player);
	void RevokePortalAbilitiesFromPlayer(APrototypeCharacter* Player);
	void ResetSequence();
	void AdvanceSequence();

	// Funciones auxiliares para el manejo de inputs
	void ResetPlayersCompletedStep();
	bool HaveAllPlayersCompletedStep() const;
	void CheckAndAdvanceSequence();

private:
	UPROPERTY()
	bool bWaitingForInput;
};
