// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemBase.h"
#include "WeaponBase.generated.h"

class UGameplayAbilityBase;
/**
 * 
 */
UCLASS()
class PROTOTYPE_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()

public:
protected:
	UPROPERTY(EditAnywhere, Category= "Ability to Grant")
	TObjectPtr<UGameplayAbilityBase> AbilityToGrant;
private:
	
};
