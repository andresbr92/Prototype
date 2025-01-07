// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterBase.h"

#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"


UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCustomAbilitySystemComponent* ACharacterBase::GetCustomAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ACharacterBase::InitializeAttributes()
{
	if (!DefaultAttributeEffect)
	{
		UE_LOG(LogTemp, Error, TEXT("DefaultAttributeEffect is not set in %s"), *GetName());
	}
	if (IsValid(AbilitySystemComponent))
	{
		FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
		ContextHandle.AddSourceObject(DefaultAttributeEffect);
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1.f, ContextHandle);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

