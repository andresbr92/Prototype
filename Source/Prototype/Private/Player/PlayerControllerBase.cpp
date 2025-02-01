// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerControllerBase.h"

#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"
#include "Characters/CharacterBase.h"
#include "Inventory/PTInventoryManagerComponent.h"
#include "Equipment/PrototypeQuickBarComponent.h"
#include "Player/PlayerStateBase.h"


APlayerControllerBase::APlayerControllerBase()
{
	
	
}

APlayerStateBase* APlayerControllerBase::GetPlayerStateBase() const
{
	return CastChecked<APlayerStateBase>(PlayerState, ECastCheckedType::NullAllowed);
}

void APlayerControllerBase::PreProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PreProcessInput(DeltaTime, bGamePaused);
}

void APlayerControllerBase::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	
	
		
		if (UCustomAbilitySystemComponent* CustomASC = GetCustomAbilitySystemComponent())
		{
		
			CustomASC->ProcessAbilityInput(DeltaTime, bGamePaused);
		}
		
	
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
	
}



UCustomAbilitySystemComponent* APlayerControllerBase::GetCustomAbilitySystemComponent() const
{
	const APlayerStateBase* PSBase = GetPlayerStateBase();
	return PSBase ? PSBase->GetCustomAbilitySystemComponent() : nullptr;
	
	
}




