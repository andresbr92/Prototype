// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PortalBase_ASC.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "GameplayAbilitySpec.h"
#include "Kismet/GameplayStatics.h"

#include "AbilitySystem/AbilitySystemComponent/PortalBaseAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/PortalAttributeSetBase.h"


// Sets default values
APortalBase_ASC::APortalBase_ASC()
{
	PrimaryActorTick.bCanEverTick = false;

	PortalAbilitySystemComponent = CreateDefaultSubobject<UPortalBaseAbilitySystemComponent>(TEXT("PortalASC"));
	PortalAbilitySystemComponent->SetIsReplicated(true);
	PortalAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	PortalAttributeSet = CreateDefaultSubobject<UPortalAttributeSetBase>(TEXT("PortalAS"));

	// Inicializar tags si es necesario en el constructor o en Blueprint Defaults.
	Tag_PortalActive = FGameplayTag::RequestGameplayTag(FName("Portal.State.Active"));
	Tag_PortalInactive = FGameplayTag::RequestGameplayTag(FName("Portal.State.Inactive"));
	Tag_MinigameActive = FGameplayTag::RequestGameplayTag(FName("Portal.State.MinigameActive"));
	Tag_InputUp = FGameplayTag::RequestGameplayTag(FName("Input.Up"));
	Tag_InputDown = FGameplayTag::RequestGameplayTag(FName("Input.Down"));
	Tag_InputLeft = FGameplayTag::RequestGameplayTag(FName("Input.Left"));
	Tag_InputRight = FGameplayTag::RequestGameplayTag(FName("Input.Right"));


	// Asegúrate de que el portal comienza inactivo
	PortalAbilitySystemComponent->AddLooseGameplayTag(Tag_PortalInactive);
}



UAbilitySystemComponent* APortalBase_ASC::GetAbilitySystemComponent() const
{
	return PortalAbilitySystemComponent;
}
void APortalBase_ASC::BeginPlay()
{
	Super::BeginPlay();

	// Otorgar Gameplay Abilities internas al portal
	for (TSubclassOf<UGameplayAbility> AbilityClass : PortalGameplayAbilities)
	{
		if (AbilityClass)
		{
			FGameplayAbilitySpec AbilitySpec(AbilityClass.GetDefaultObject(), 1, 0);
			PortalAbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}
}
void APortalBase_ASC::StartPortalMinigame()
{
	if (bMinigameActive) return; // No reiniciar si ya está activo

	if (PlayersOnBases.Num() >= NumPlayersNeeded)
	{
		bMinigameActive = true;
		PortalAbilitySystemComponent->AddLooseGameplayTag(Tag_MinigameActive);
		PortalAbilitySystemComponent->RemoveLooseGameplayTag(Tag_PortalInactive);
		PortalAbilitySystemComponent->AddLooseGameplayTag(Tag_PortalActive);

		GenerateSequence();
		SequenceIndex = 0;
		ResetPlayerInputTracking();
		StartSequenceTimer();

		// UE_LOG(LogTemp, Warning, TEXT("Minigame Started. Sequence: %s"), *FString::Join(GetCurrentSequenceForDisplay().ToArray(), TEXT(", ")));

		// Aquí podrías activar UI para mostrar la secuencia a los jugadores
		// y comenzar la cuenta atrás visual.
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough players on bases to start minigame."));
	}
}
void APortalBase_ASC::ProcessPlayerInput(APawn* Player, FGameplayTag InputTag)
{
	if (!bMinigameActive) return;
	if (!PlayerInputtedThisStep.Contains(Player)) return; // Solo registrar el primer input por jugador por paso

	if (PlayerInputtedThisStep[Player]) return; // Jugador ya ha dado input en este paso

	PlayerInputtedThisStep[Player] = true;

	UE_LOG(LogTemp, Warning, TEXT("Player %s Input: %s"), *Player->GetName(), *InputTag.ToString());

	if (CurrentSequence.IsValidIndex(SequenceIndex) && CurrentSequence[SequenceIndex] == InputTag)
	{
		UE_LOG(LogTemp, Warning, TEXT("Correct Input!"));
		if (CheckAllPlayersInputted())
		{
			AdvanceSequence();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Incorrect Input!"));
		FailSequence();
	}
}

void APortalBase_ASC::ResetSequence()
{
	UE_LOG(LogTemp, Warning, TEXT("Sequence Reset!"));
	SequenceIndex = 0;
	GenerateSequence(); // Opcional: Regenerar secuencia en cada reinicio
	ResetPlayerInputTracking();
	StartSequenceTimer();
	ApplyDamageToPortal();

	// UE_LOG(LogTemp, Warning, TEXT("New Sequence: %s"), *FString::Join(GetCurrentSequenceForDisplay().ToArray(), TEXT(", ")));
	// Aquí podrías actualizar la UI para mostrar la secuencia reiniciada
}

void APortalBase_ASC::AdvanceSequence()
{
	SequenceIndex++;
	ResetPlayerInputTracking();

	if (CheckSequenceCompletion())
	{
		CompleteSequence();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Sequence Advanced to step: %d"), SequenceIndex + 1);
		StartSequenceTimer();
		// Aquí podrías actualizar la UI para mostrar el siguiente paso de la secuencia
	}
}

void APortalBase_ASC::FailSequence()
{
	ResetSequence();
}

void APortalBase_ASC::CompleteSequence()
{
	UE_LOG(LogTemp, Warning, TEXT("Sequence Completed! Portal Closed!"));
	bMinigameActive = false;
	PortalAbilitySystemComponent->RemoveLooseGameplayTag(Tag_MinigameActive);
	PortalAbilitySystemComponent->RemoveLooseGameplayTag(Tag_PortalActive);
	PortalAbilitySystemComponent->AddLooseGameplayTag(Tag_PortalInactive);

	ClosePortal();
	// Aquí podrías activar efectos visuales de cierre del portal
}

float APortalBase_ASC::GetPortalIntegrity() const
{
	// return PortalAttributeSet->GetPortalIntegrity();
	return 1.f;
}

TArray<FGameplayTag> APortalBase_ASC::GetCurrentSequenceForDisplay() const
{
	TArray<FGameplayTag> DisplaySequence;
	
	for (const FGameplayTag& Tag : CurrentSequence)
	{
		if (Tag == Tag_InputUp) DisplaySequence.Add(Tag_InputUp);
		else if (Tag == Tag_InputDown) DisplaySequence.Add(Tag_InputDown);
		else if (Tag == Tag_InputLeft) DisplaySequence.Add(Tag_InputLeft);
		else if (Tag == Tag_InputRight) DisplaySequence.Add(Tag_InputRight);
	}
	return DisplaySequence;
}


void APortalBase_ASC::AddPlayerToBase(APawn* PlayerPawn)
{
	if (!PlayersOnBases.Contains(PlayerPawn))
	{
		PlayersOnBases.Add(PlayerPawn);
		UE_LOG(LogTemp, Warning, TEXT("Player %s entered base. Players on bases: %d"), *PlayerPawn->GetName(), PlayersOnBases.Num());
		if (!bMinigameActive && PlayersOnBases.Num() >= NumPlayersNeeded)
		{
			StartPortalMinigame(); // Iniciar automáticamente si suficientes jugadores están en las bases
		}
	}
}

void APortalBase_ASC::RemovePlayerFromBase(APawn* PlayerPawn)
{
	if (PlayersOnBases.Contains(PlayerPawn))
	{
		PlayersOnBases.Remove(PlayerPawn);
		UE_LOG(LogTemp, Warning, TEXT("Player %s left base. Players on bases: %d"), *PlayerPawn->GetName(), PlayersOnBases.Num());
		if (bMinigameActive && PlayersOnBases.Num() < NumPlayersNeeded)
		{
			// Opcional: Pausar o cancelar el minijuego si jugadores se van
			UE_LOG(LogTemp, Warning, TEXT("Minigame might be interrupted as players left bases."));
			// FailSequence(); // O podrías tener lógica para pausar en lugar de fallar inmediatamente
		}
	}
}


void APortalBase_ASC::GenerateSequence()
{
	CurrentSequence.Reset();
	int32 SequenceLength = 5; // Longitud de la secuencia, ajustable
	TArray<FGameplayTag> PossibleTags;
	if (InputTags.HasTag(Tag_InputUp)) PossibleTags.Add(Tag_InputUp);
	if (InputTags.HasTag(Tag_InputDown)) PossibleTags.Add(Tag_InputDown);
	if (InputTags.HasTag(Tag_InputLeft)) PossibleTags.Add(Tag_InputLeft);
	if (InputTags.HasTag(Tag_InputRight)) PossibleTags.Add(Tag_InputRight);


	if (PossibleTags.Num() > 0)
	{
		for (int32 i = 0; i < SequenceLength; ++i)
		{
			CurrentSequence.Add(PossibleTags[FMath::RandRange(0, PossibleTags.Num() - 1)]);
		}
	}
}

void APortalBase_ASC::StartSequenceTimer()
{
	GetWorldTimerManager().SetTimer(SequenceTimerHandle, this, &APortalBase_ASC::OnSequenceTimerExpired, SequenceTimeLimit, false);
}

void APortalBase_ASC::OnSequenceTimerExpired()
{
	UE_LOG(LogTemp, Warning, TEXT("Sequence Timer Expired!"));
	FailSequence(); // Si el tiempo se agota, falla la secuencia
}

bool APortalBase_ASC::CheckSequenceCompletion()
{
	return SequenceIndex >= CurrentSequence.Num();
}

bool APortalBase_ASC::CheckAllPlayersInputted()
{
	for (auto const& [Player, Inputted] : PlayerInputtedThisStep)
	{
		if (!Inputted && PlayersOnBases.Contains(Player)) // Solo revisar jugadores que aún están en las bases
		{
			return false; // Al menos un jugador no ha dado input
		}
	}
	return true; // Todos los jugadores en las bases han dado input
}


void APortalBase_ASC::ApplyDamageToPortal()
{
	if (GE_DamagePortal_Class)
	{
		FGameplayEffectSpecHandle GESpecHandle = PortalAbilitySystemComponent->MakeOutgoingSpec(GE_DamagePortal_Class, 1, PortalAbilitySystemComponent->MakeEffectContext());
		if (GESpecHandle.IsValid())
		{
			PortalAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GESpecHandle.Data.Get());
		}
	}
}

void APortalBase_ASC::ClosePortal()
{
	// Aquí puedes añadir lógica visual o de juego para cerrar el portal
	UE_LOG(LogTemp, Warning, TEXT("Portal is closing..."));
	Destroy(); // Ejemplo: Destruir el portal actor
}

void APortalBase_ASC::ResetPlayerInputTracking()
{
	PlayerInputtedThisStep.Reset();
	for (APawn* Player : PlayersOnBases)
	{
		PlayerInputtedThisStep.Add(Player, false); // Inicializar a false para cada jugador en las bases
	}
}


