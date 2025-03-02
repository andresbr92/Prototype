// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSets/AttributeSetBase.h"

#include "Net/UnrealNetwork.h"


UAttributeSetBase::UAttributeSetBase()
{
}

void UAttributeSetBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, StaminaRegenRate, COND_None, REPNOTIFY_Always);


}

void UAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UAttributeSetBase::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
}

void UAttributeSetBase::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
}

void UAttributeSetBase::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
}

void UAttributeSetBase::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue,
	float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
}

void UAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, Health, OldHealth);
}
void UAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, MaxHealth, OldMaxHealth);
}
void UAttributeSetBase::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, Stamina, OldStamina);
}

void UAttributeSetBase::OnRep_Age(const FGameplayAttributeData& OldAge)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, Age, OldAge);
}

void UAttributeSetBase::OnRep_MaxAge(const FGameplayAttributeData& OldMaxAge)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, MaxAge, OldMaxAge);
}

void UAttributeSetBase::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, MaxStamina, OldMaxStamina);
}

void UAttributeSetBase::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, MoveSpeed, OldMoveSpeed);
}

void UAttributeSetBase::OnRep_StaminaRegenRate(const FGameplayAttributeData& OldStaminaRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, StaminaRegenRate, OldStaminaRegenRate);
}

