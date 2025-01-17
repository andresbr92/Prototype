// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerControllerBase.h"

#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"
#include "Characters/CharacterBase.h"
#include "Inventory/PTInventoryManagerComponent.h"
#include "Player/PlayerStateBase.h"


APlayerControllerBase::APlayerControllerBase()
{
	InventoryManagerComponent = CreateDefaultSubobject<UPTInventoryManagerComponent>("InventoryManager");
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
	UCustomAbilitySystemComponent* CustomASC = GetPlayerStateBase()->GetCustomAbilitySystemComponent();
	if (CustomASC)
	{
		
		CustomASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	Super::PostProcessInput(DeltaTime, bGamePaused);
	
}
