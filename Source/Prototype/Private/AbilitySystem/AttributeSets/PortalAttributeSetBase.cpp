// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/PortalAttributeSetBase.h"

#include "Net/UnrealNetwork.h"

UPortalAttributeSetBase::UPortalAttributeSetBase()
{
}

void UPortalAttributeSetBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UPortalAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UPortalAttributeSetBase::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void UPortalAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UPortalAttributeSetBase::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UPortalAttributeSetBase::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue,
	float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UPortalAttributeSetBase::OnRep_PortalIntegrity(const FGameplayAttributeData& OldPortalIntegrity)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UPortalAttributeSetBase, PortalIntegrity, OldPortalIntegrity);
}
