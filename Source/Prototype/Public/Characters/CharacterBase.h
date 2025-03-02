// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "AbilitySystem/PrototypeAbilitySet.h"
#include "CharacterBase.generated.h"
class UPrototypeAbilitySet;

UENUM(BlueprintType)
enum class EPTHitReactDirection : uint8
{
	// 0
	None			UMETA(DisplayName = "None"),
	// 1
	Left 			UMETA(DisplayName = "Left"),
	// 2
	Front 			UMETA(DisplayName = "Front"),
	// 3
	Right			UMETA(DisplayName = "Right"),
	// 4
	Back			UMETA(DisplayName = "Back")
};

class UAttributeSetBase;
class UCustomAbilitySystemComponent;
class UAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterBaseHitReactDelegate, EPTHitReactDirection, Direction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ACharacterBase*, Character);
/**
* The base Character class for the game. Everything with an AbilitySystemComponent in this game will inherit from this class.
* This class should not be instantiated and instead subclassed.
*/
UCLASS()
class PROTOTYPE_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase(const class FObjectInitializer& ObjectInitializer);
	
	// TODO:  Set the Hit React direction in the Animation Blueprint 
	// UPROPERTY(BlueprintAssignable, Category = "Prototype | PTCharacter")
	// FCharacterBaseHitReactDelegate ShowHitReact;
	
	UPROPERTY(BlueprintAssignable, Category = "Prototype | PTCharacter")
	FCharacterDiedDelegate OnCharacterDied;
	
	// Implement the IAbilitySystemInterface. (To find the Ability SystemComponent)
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UCustomAbilitySystemComponent* GetCustomAbilitySystemComponent() const;
	
	UFUNCTION(BlueprintPure, Category = "Ability System")
	UAttributeSetBase* GetAttributeSet() const { return AttributeSetBase; };

	UFUNCTION(BlueprintCallable, Category = "Prototype | PTCharacter")
	virtual bool IsAlive() const;
	// Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too.
	virtual void RemoveCharacterAbilities();
	
	// UFUNCTION(BlueprintCallable)
	// EPTHitReactDirection GetHitReactDirection(const FVector& ImpactPoint);
	// TODO: Implement hit reaction
	// UFUNCTION(NetMulticast, Reliable, WithValidation)
	// virtual void PlayHitReact(FGameplayTag HitDirection, AActor* DamageCauser);
	// virtual void PlayHitReact_Implementation(FGameplayTag HitDirection, AActor* DamageCauser);
	// virtual bool PlayHitReact_Validate(FGameplayTag HitDirection, AActor* DamageCauser);
	
	
	

protected:
	UPROPERTY()
	TObjectPtr<UCustomAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSetBase> AttributeSetBase;

	
	FGameplayTag HitDirectionFrontTag;
	FGameplayTag HitDirectionBackTag;
	FGameplayTag HitDirectionRightTag;
	FGameplayTag HitDirectionLeftTag;
	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Prototype | PTCharacter")
	FText CharacterName;

	// Death Animation
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Prototype | PTCharacter")
	UAnimMontage* DeathMontage;
	
	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	// // In PTCharacter there is another sets of specific abilities
	// UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Prototype|Abilities")
	// TArray<TSubclassOf<class UGameplayAbilityBase>> CharacterAbilities;

	UPROPERTY(EditDefaultsOnly, Category= "Prototype|Abilities")
	TObjectPtr<UPrototypeAbilitySet> AbilitySet;
	
	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Prototype|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;
	
	// Gameplay effect to initialize attributes
	UPROPERTY(EditDefaultsOnly, Category = "Prototype|Abilities")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

	UPROPERTY()
	FPrototypeAbilitySet_GrantedHandles GrantedAbilityHandles;

	

	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();
	// Initialize the Character's attributes. Must run on Server, but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes();
	virtual void AddStartupEffects();
	
	


public:
	/**
	* Getters for attributes from GDAttributeSetBase
	**/
	UFUNCTION(BlueprintCallable, Category = "Prototype|PTCharacter|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Prototype|PTCharacter|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Prototype|PTCharacter|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Prototype|PTCharacter|Attributes")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Prototype|PTCharacter|Attributes")
	float GetAge() const;

	UFUNCTION(BlueprintCallable, Category = "Prototype|PTCharacter|Attributes")
	float GetMaxAge() const;

	UFUNCTION(BlueprintCallable, Category = "Prototype|PTCharacter|Attributes")
	float GetStaminaRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "Prototype|PTCharacter|Attributes")
	float GetMoveSpeed() const;

	// Gets the Base value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "Prototype|PTCharacter|Attributes")
	float GetMoveSpeedBaseValue() const;

	virtual void Die();

	UFUNCTION(BlueprintCallable, Category = "Prototype|PTCharacter")
	virtual void FinishDying();


	

};
