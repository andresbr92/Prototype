// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerStateBase.h"
#include "Items/PortalBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/AttributeSetBase.h"

APlayerStateBase::APlayerStateBase()
{
	SetNetUpdateFrequency(100.f);
	AbilitySystemComponent = CreateDefaultSubobject<UCustomAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AttributeSet = CreateDefaultSubobject<UAttributeSetBase>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* APlayerStateBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSetBase * APlayerStateBase::GetAttributeSetBase() const
{
	return AttributeSet.Get();
}

UCustomAbilitySystemComponent* APlayerStateBase::GetCustomAbilitySystemComponent() const
{
	return CastChecked<UCustomAbilitySystemComponent>(AbilitySystemComponent, ECastCheckedType::NullAllowed);
}

void APlayerStateBase::SetPortalInteractingWith(APortalBase* Portal)
{
	PortalInteractingWith = Portal;
}

APortalBase* APlayerStateBase::GetPortalInteractingWith() const
{
	return PortalInteractingWith;
}

float APlayerStateBase::GetMoveSpeed() const
{
	return AttributeSet->GetMoveSpeed();
}

