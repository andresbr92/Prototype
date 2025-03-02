// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CharacterBase.h"

#include "AbilitySystem/PrototypeAbilitySet.h"
#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/AttributeSetBase.h"
#include "Characters/PrototypeCharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


ACharacterBase::ACharacterBase(const class FObjectInitializer& ObjectInitializer) :
Super(ObjectInitializer.SetDefaultSubobjectClass<UPrototypeCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bAlwaysRelevant = true;

	// Cache tags
	HitDirectionFrontTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Front"));
	HitDirectionBackTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Back"));
	HitDirectionRightTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Right"));
	HitDirectionLeftTag = FGameplayTag::RequestGameplayTag(FName("Effect.HitReact.Left"));
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
	EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("Effect.RemoveOnDeath"));
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UCustomAbilitySystemComponent* ACharacterBase::GetCustomAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

bool ACharacterBase::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void ACharacterBase::RemoveCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponent) || !AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}
	
	GrantedAbilityHandles.TakeFromAbilitySystem(AbilitySystemComponent);

	AbilitySystemComponent->bCharacterAbilitiesGiven = false;
	
}

void ACharacterBase::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponent) || AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}
	if (AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &GrantedAbilityHandles, this);
	}
}

void ACharacterBase::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !IsValid(AbilitySystemComponent) || AbilitySystemComponent->bStartupEffectsApplied)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
		}
	}

	AbilitySystemComponent->bStartupEffectsApplied = true;
}

void ACharacterBase::InitializeAttributes()
{
	// if (StartupEffects.IsEmpty())
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("DefaultAttributeEffect is not set in %s"), *GetName());
	// }
	//
	// for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
	// {
	// 	
	// 	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	// 	ContextHandle.AddSourceObject(GameplayEffect);
	// 	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1.f, ContextHandle);
	// 	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	// }
	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}
	if (!DefaultAttributes)
	{
		return;
	}
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
	
}

float ACharacterBase::GetHealth() const
{
	if(IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetHealth();
	}
	return 0.0f;
}

float ACharacterBase::GetMaxHealth() const
{
	if(IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMaxHealth();
	}
	return 0.0f;
}

float ACharacterBase::GetStamina() const
{
	if(IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetStamina();
	}
	return 0.0f;
}

float ACharacterBase::GetMaxStamina() const
{
	if(IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMaxStamina();
	}
	return 0.0f;
}

float ACharacterBase::GetAge() const
{
	if(IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetAge();
	}
	return 0.0f;
}

float ACharacterBase::GetMaxAge() const
{
	if(IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMaxAge();
	}
	return 0.0f;
}

float ACharacterBase::GetStaminaRegenRate() const
{
	if(IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetStaminaRegenRate();
	}
	return 0.0f;
}

float ACharacterBase::GetMoveSpeed() const
{
	if(IsValid(AttributeSetBase))
	{
		return AttributeSetBase->GetMoveSpeed();
	}
	return 0.0f;
}

float ACharacterBase::GetMoveSpeedBaseValue() const
{
	if (AttributeSetBase)
	{
		return AttributeSetBase->GetMoveSpeedAttribute().GetGameplayAttributeData(AttributeSetBase.Get())->GetBaseValue();
	}

	return 0.0f;
}
// Run on Server and all clients
void ACharacterBase::Die()
{
	// only runs on server
	RemoveCharacterAbilities();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);
	OnCharacterDied.Broadcast(this);

	// Cancel all abilities actually performing
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->CancelAbilities();
		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);
		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}
	if (DeathMontage)
	{
		PlayAnimMontage(DeathMontage);
	}
	else
	{
		FinishDying();
	}
	
}

void ACharacterBase::FinishDying()
{
	Destroy();
}

