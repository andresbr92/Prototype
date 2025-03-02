// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "PrototypeEnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API APrototypeEnemyBase : public ACharacterBase
{
	GENERATED_BODY()
public:
	APrototypeEnemyBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
protected:
	UPROPERTY()
	class UCustomAbilitySystemComponent* HardRefAbilitySystemComponent;
	UPROPERTY()
	class UAttributeSetBase* HardRefAttributeSetBase;
	// TODO: UI status bar 
	// UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Prototype|UI")
	// TSubclassOf<class >
	
	virtual void BeginPlay() override;
	FDelegateHandle HealthChangedDelegateHandle;
	// Attribute changed callbacks
	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	// Tag change callbacks
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void InitializeAttributes() override;
	
};
