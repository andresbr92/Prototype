// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Interaction/IInteractableTarget.h"
#include "GameFramework/Actor.h"
#include "CustomPhysics/PrototypeCollisionsChannel.h"
#include "ItemBase.generated.h"

class UGameplayAbilityBase;
class USphereComponent;

UCLASS()
class PROTOTYPE_API AItemBase : public AActor, public IInteractableTarget
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();
	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	FInteractionOption Option;

public:
	virtual void GatherInteractionOptions(const FInteractionQuery& InteractQuery, FInteractionOptionBuilder& InteractionBuilder) override;
	// UPROPERTY(EditAnywhere);
	// TObjectPtr<UStaticMeshComponent> Mesh;
	// UPROPERTY(VisibleAnywhere)
	// TObjectPtr<USphereComponent> Sphere;
	// UPROPERTY(EditAnywhere, Category = "Ability System")
	// TSubclassOf<UGameplayAbilityBase> InteractionAbility;
	
	

};
