// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "PlayerStateBase.generated.h"

class UAttributeSet;
/**
 * 
 */
UCLASS()
class PROTOTYPE_API APlayerStateBase : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	APlayerStateBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; };
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "Ability System")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "Ability System")
	TObjectPtr<UAttributeSet> AttributeSet;

	


	

	
private:
	
	
};
