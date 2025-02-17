// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "AbilitySystemInterface.h"
#include "PortalBase_ASC.generated.h"

class UPortalAttributeSetBase;
class UPortalBaseAbilitySystemComponent;

UCLASS()
class PROTOTYPE_API APortalBase_ASC : public AItemBase, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APortalBase_ASC();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UPortalBaseAbilitySystemComponent* PortalAbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	UPortalAttributeSetBase* PortalAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tags")
	FGameplayTagContainer SequenceTags; // Secuencia de tags para cerrar el portal

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tags")
	FGameplayTagContainer InputTags; // Tags de input permitidos (Up, Down, Left, Right)

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tags")
	FGameplayTag Tag_PortalActive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tags")
	FGameplayTag Tag_PortalInactive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tags")
	FGameplayTag Tag_MinigameActive;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tags")
	FGameplayTag Tag_InputUp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tags")
	FGameplayTag Tag_InputDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tags")
	FGameplayTag Tag_InputLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Tags")
	FGameplayTag Tag_InputRight;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Abilities")
	TArray<TSubclassOf<UGameplayAbility>> PortalGameplayAbilities; // Habilidades internas del portal

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> GE_InputUp_Class;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> GE_InputDown_Class;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> GE_InputLeft_Class;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> GE_InputRight_Class;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay Effects")
	TSubclassOf<UGameplayEffect> GE_DamagePortal_Class; // GE para reducir la integridad del portal


	// Bases para jugadores (puedes usar USceneComponent como hijos en Blueprint)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bases", meta = (AllowPrivateAccess = "true"))
	TArray<USceneComponent*> PlayerBases;

	int32 NumPlayersNeeded = 2; // Ejemplo: 2 jugadores requeridos

	TArray<APawn*> PlayersOnBases; // Lista de jugadores actualmente en las bases

	TArray<FGameplayTag> CurrentSequence; // Secuencia actual activa
	int32 SequenceIndex = 0; // Indice actual en la secuencia

	FTimerHandle SequenceTimerHandle;
	float SequenceTimeLimit = 3.0f; // Tiempo límite para cada input de la secuencia

	TMap<APawn*, bool> PlayerInputtedThisStep; // Mapa para rastrear qué jugadores han dado input en el paso actual

	bool bMinigameActive = false;

public:
	// AbilitySystemInterface
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Portal")
	void StartPortalMinigame();

	UFUNCTION(BlueprintCallable, Category = "Portal")
	void ProcessPlayerInput(APawn* Player, FGameplayTag InputTag);

	UFUNCTION(BlueprintCallable, Category = "Portal")
	void ResetSequence();

	UFUNCTION(BlueprintCallable, Category = "Portal")
	void AdvanceSequence();

	UFUNCTION(BlueprintCallable, Category = "Portal")
	void FailSequence();

	UFUNCTION(BlueprintCallable, Category = "Portal")
	void CompleteSequence();

	UFUNCTION(BlueprintCallable, Category = "Portal")
	float GetPortalIntegrity() const;

	UFUNCTION(BlueprintCallable, Category = "Portal")
	bool IsMinigameActive() const { return bMinigameActive; }

	UFUNCTION(BlueprintCallable, Category = "Portal")
	TArray<FGameplayTag> GetCurrentSequenceForDisplay() const;

	UFUNCTION(BlueprintCallable, Category = "Portal")
	void AddPlayerToBase(APawn* PlayerPawn);

	UFUNCTION(BlueprintCallable, Category = "Portal")
	void RemovePlayerFromBase(APawn* PlayerPawn);

	UFUNCTION(BlueprintCallable, Category = "Portal")
	int32 GetNumPlayersOnBases() const { return PlayersOnBases.Num(); }
protected:
	void GenerateSequence();
	void StartSequenceTimer();
	void OnSequenceTimerExpired();
	bool CheckSequenceCompletion();
	bool CheckAllPlayersInputted();
	void ApplyDamageToPortal();
	void ClosePortal();
	void ResetPlayerInputTracking();
};
