// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "PlayerStateBase.generated.h"

class UCustomAbilitySystemComponent;
class UAttributeSetBase;
class APortalBase;
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
	class UAttributeSetBase* GetAttributeSetBase() const;
	virtual UCustomAbilitySystemComponent* GetCustomAbilitySystemComponent() const;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APortalBase> PortalInteractingWith;
	UFUNCTION(BlueprintCallable, Category="Prototype|PlayerState")
	void SetPortalInteractingWith(APortalBase* Portal);
	UFUNCTION(BlueprintCallable, Category="Prototype|PlayerState")
	APortalBase* GetPortalInteractingWith() const;
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "Ability System")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category= "Ability System")
	TObjectPtr<UAttributeSetBase> AttributeSet;

	


	

	
public:
	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
	float GetMoveSpeed() const;
	
	
};
