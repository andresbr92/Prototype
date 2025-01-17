// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/PrototypeAbilitySet.h"

#include "GameplayAbilitySpecHandle.h"

#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"
#include "AbilitySystem/GameplayAbilities/GameplayAbilityBase.h"

void FPrototypeAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FPrototypeAbilitySet_GrantedHandles::TakeFromAbilitySystem(UCustomAbilitySystemComponent* BaseASC)
{
	check(BaseASC);
	if (!BaseASC->IsOwnerActorAuthoritative())
	{
		return;
	}
		

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			BaseASC->ClearAbility(Handle);
		}
	}
	AbilitySpecHandles.Reset();
}

UPrototypeAbilitySet::UPrototypeAbilitySet(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void UPrototypeAbilitySet::GiveToAbilitySystem(UAbilitySystemComponent* BaseASC,
	FPrototypeAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject) const
{
	check(BaseASC);
	if (!BaseASC->IsOwnerActorAuthoritative())
	{
		return;
	}
		// Grant the gameplay abilities.
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FPrototypeAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UGameplayAbilityBase* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UGameplayAbilityBase>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = BaseASC->GiveAbility(AbilitySpec);

		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}
	
}
