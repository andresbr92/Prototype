// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemBase.h"

#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"
#include "Characters/PrototypeCharacter.h"
#include "Components/SphereComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ItemBase)
// Sets default values
struct FInteractionQuery;
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	// RootComponent = Mesh;
	//
	// Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	// Sphere->SetupAttachment(GetRootComponent());

}

void AItemBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// APrototypeCharacter* Character = Cast<APrototypeCharacter>(OtherActor);
	// if (!Character) return;
	// Character->SetOverlappingItem(this);
	// UCustomAbilitySystemComponent* AbilitySystemComponent = Character->GetCustomAbilitySystemComponent();
	// if (AbilitySystemComponent)
	// {
	// 	AbilitySystemComponent->GrantInteractionAbility(InteractionAbility);
	// }
	
}

void AItemBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// APrototypeCharacter* Character = Cast<APrototypeCharacter>(OtherActor);
	// if (!Character) return;
	// Character->SetOverlappingItem(nullptr);
	// UCustomAbilitySystemComponent* AbilitySystemComponent = Character->GetCustomAbilitySystemComponent();
	// if (AbilitySystemComponent)
	// {
	// 	AbilitySystemComponent->ClearInteractionAbility();
	// }
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	// Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnSphereBeginOverlap);
	// Sphere->OnComponentEndOverlap.AddDynamic(this, &AItemBase::OnSphereEndOverlap);
	
	
	
}

void AItemBase::GatherInteractionOptions(const FInteractionQuery& InteractQuery,
	FInteractionOptionBuilder& InteractionBuilder)
{
	InteractionBuilder.AddInteractionOption(Option);
}

// Called every frame


