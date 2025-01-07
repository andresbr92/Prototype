// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_API AEnemy : public ACharacterBase
{
	GENERATED_BODY()
public:
	AEnemy();
	virtual void BeginPlay() override;
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void InitializeAttributes() override;
	
};
