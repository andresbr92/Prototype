// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PortalAttributeSetBase.generated.h"

/**
 * 
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
UCLASS()
class PROTOTYPE_API UPortalAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()
public:
	UPortalAttributeSetBase();
	
	// Attributes to replicate
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	// Functions overrides
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

	UPROPERTY(BlueprintReadOnly, Category="Portal|Attributes", ReplicatedUsing= OnRep_PortalIntegrity)
	FGameplayAttributeData PortalIntegrity;
	ATTRIBUTE_ACCESSORS(UPortalAttributeSetBase, PortalIntegrity)


protected:
	UFUNCTION()
	virtual void OnRep_PortalIntegrity(const FGameplayAttributeData& OldPortalIntegrity);
	
};
