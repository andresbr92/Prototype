// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PortalBase.h"
#include "Net/UnrealNetwork.h"
#include "Characters/PrototypeCharacter.h"
#include "AbilitySystemComponent.h"
#include "Player/PlayerStateBase.h"
// #include "GameFramework/GameplayMessageSubsystem.h"

// Definir categoría de log para el portal
DEFINE_LOG_CATEGORY_STATIC(LogPortalBase, Log, All);

// Sets default values
APortalBase::APortalBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	// Valores por defecto
	TimeLimitPerInput = 1.0f;
	MaxPlayers = 4;
	CurrentSequenceIndex = 0;
	bIsSequenceActive = false;
	bWaitingForInput = false;

	// Crear y configurar el AbilitySystemComponent
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	
}

void APortalBase::BeginPlay()
{
	Super::BeginPlay();

	// Asegurarse de que el servidor inicialice el ASC
	if (HasAuthority())
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		UE_LOG(LogPortalBase, Log, TEXT("Portal [%s] - BeginPlay - ASC Initialized on Server"), *GetName());
	}
	else
	{
		UE_LOG(LogPortalBase, Log, TEXT("Portal [%s] - BeginPlay - Client Instance"), *GetName());
	}
}

void APortalBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UAbilitySystemComponent* APortalBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APortalBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, CharactersPlaying);
	DOREPLIFETIME(ThisClass, TagsSequence);
	DOREPLIFETIME(ThisClass, CurrentSequenceIndex);
	DOREPLIFETIME(ThisClass, bIsSequenceActive);
}

void APortalBase::StartPortalSequence()
{
	if (!HasAuthority())
	{
		UE_LOG(LogPortalBase, Warning, TEXT("Portal [%s] - Attempted to start sequence on client"), *GetName());
		return;
	}

	if (CharactersPlaying.Num() == 0)
	{
		UE_LOG(LogPortalBase, Warning, TEXT("Portal [%s] - Cannot start sequence: No players"), *GetName());
		return;
	}

	if (TagsSequence.Num() == 0)
	{
		UE_LOG(LogPortalBase, Warning, TEXT("Portal [%s] - Cannot start sequence: No tags in sequence"), *GetName());
		return;
	}

	bIsSequenceActive = true;
	ResetSequence();
	OnRep_IsSequenceActive();

	UE_LOG(LogPortalBase, Log, TEXT("Portal [%s] - Sequence Started with %d players"), *GetName(), CharactersPlaying.Num());
}

void APortalBase::StopPortalSequence()
{
	if (!HasAuthority())
	{
		UE_LOG(LogPortalBase, Warning, TEXT("Portal [%s] - Attempted to stop sequence on client"), *GetName());
		return;
	}

	bIsSequenceActive = false;
	GetWorld()->GetTimerManager().ClearTimer(InputTimerHandle);
	OnRep_IsSequenceActive();

	UE_LOG(LogPortalBase, Log, TEXT("Portal [%s] - Sequence Stopped"), *GetName());
}

void APortalBase::HandleInputReceived(const FGameplayTag& InputTag, APrototypeCharacter* InputPlayer)
{
	if (!bIsSequenceActive || !bWaitingForInput)
	{
		UE_LOG(LogPortalBase, Verbose, TEXT("Portal [%s] - Input Rejected: Sequence %s, Waiting %s"), 
			*GetName(), 
			bIsSequenceActive ? TEXT("Active") : TEXT("Inactive"),
			bWaitingForInput ? TEXT("True") : TEXT("False"));
		return;
	}

	if (!InputPlayer)
	{
		UE_LOG(LogPortalBase, Error, TEXT("Portal [%s] - Input Received with null player"), *GetName());
		return;
	}

	UE_LOG(LogPortalBase, Log, TEXT("Portal [%s] - Input Received from player [%s]: %s"), 
		*GetName(), *InputPlayer->GetName(), *InputTag.ToString());
	Server_ProcessInput(InputTag, InputPlayer);
}

void APortalBase::Server_ProcessInput_Implementation(const FGameplayTag& InputTag, APrototypeCharacter* InputPlayer)
{
	if (!bIsSequenceActive || !bWaitingForInput || CurrentSequenceIndex >= TagsSequence.Num())
	{
		UE_LOG(LogPortalBase, Warning, TEXT("Portal [%s] - Server rejected input: Active=%s, Waiting=%s, Index=%d/%d"),
			*GetName(),
			bIsSequenceActive ? TEXT("True") : TEXT("False"),
			bWaitingForInput ? TEXT("True") : TEXT("False"),
			CurrentSequenceIndex,
			TagsSequence.Num());
		return;
	}

	if (!InputPlayer)
	{
		UE_LOG(LogPortalBase, Error, TEXT("Portal [%s] - Server received input with null player"), *GetName());
		return;
	}

	if (!CharactersPlaying.Contains(InputPlayer))
	{
		UE_LOG(LogPortalBase, Warning, TEXT("Portal [%s] - Received input from player [%s] who is not in the game"),
			*GetName(), *InputPlayer->GetName());
		return;
	}

	bool bWasCorrect = (InputTag == TagsSequence[CurrentSequenceIndex]);
	UE_LOG(LogPortalBase, Log, TEXT("Portal [%s] - Processing Input from player [%s]: Expected=%s, Received=%s, Correct=%s"),
		*GetName(),
		*InputPlayer->GetName(),
		*TagsSequence[CurrentSequenceIndex].ToString(),
		*InputTag.ToString(),
		bWasCorrect ? TEXT("Yes") : TEXT("No"));

	OnInputReceived.Broadcast(InputTag, bWasCorrect);

	if (bWasCorrect)
	{
		// Marcar este jugador como completado para este paso
		PlayersCompletedCurrentStep.Add(InputPlayer, true);
		
		// Verificar si todos los jugadores han completado este paso
		CheckAndAdvanceSequence();
	}
	else
	{
		// Si algún jugador falla, toda la secuencia falla
		StopPortalSequence();
		OnSequenceCompleted.Broadcast(false);
	}
}

void APortalBase::ResetPlayersCompletedStep()
{
	PlayersCompletedCurrentStep.Empty();
	for (auto Player : CharactersPlaying)
	{
		if (Player)
		{
			PlayersCompletedCurrentStep.Add(Player, false);
		}
	}
	
	UE_LOG(LogPortalBase, Verbose, TEXT("Portal [%s] - Reset completion status for %d players"), 
		*GetName(), PlayersCompletedCurrentStep.Num());
}

bool APortalBase::HaveAllPlayersCompletedStep() const
{
	if (PlayersCompletedCurrentStep.Num() != CharactersPlaying.Num())
	{
		return false;
	}

	for (const auto& KVP : PlayersCompletedCurrentStep)
	{
		if (!KVP.Value)
		{
			return false;
		}
	}

	return true;
}

void APortalBase::CheckAndAdvanceSequence()
{
	if (HaveAllPlayersCompletedStep())
	{
		UE_LOG(LogPortalBase, Log, TEXT("Portal [%s] - All players completed step %d"), 
			*GetName(), CurrentSequenceIndex);
		AdvanceSequence();
	}
	else
	{
		UE_LOG(LogPortalBase, Verbose, TEXT("Portal [%s] - Waiting for more players to complete step %d"), 
			*GetName(), CurrentSequenceIndex);
	}
}

void APortalBase::ResetSequence()
{
	UE_LOG(LogPortalBase, Log, TEXT("Portal [%s] - Resetting sequence"), *GetName());
	CurrentSequenceIndex = -1; // Lo cambiamos a -1 porque AdvanceSequence lo incrementará a 0
	ResetPlayersCompletedStep();
	AdvanceSequence();
}

void APortalBase::AdvanceSequence()
{
	if (!bIsSequenceActive)
	{
		UE_LOG(LogPortalBase, Verbose, TEXT("Portal [%s] - Cannot advance sequence: Not active"), *GetName());
		return;
	}

	// Incrementar el índice
	CurrentSequenceIndex++;

	// Verificar si hemos completado la secuencia
	if (CurrentSequenceIndex >= TagsSequence.Num())
	{
		UE_LOG(LogPortalBase, Log, TEXT("Portal [%s] - Sequence completed successfully"), *GetName());
		StopPortalSequence();
		OnSequenceCompleted.Broadcast(true);
		return;
	}

	// Resetear el estado de completado para todos los jugadores en este nuevo paso
	ResetPlayersCompletedStep();

	UE_LOG(LogPortalBase, Log, TEXT("Portal [%s] - Advancing to sequence index %d/%d, Expected tag: %s"),
		*GetName(), CurrentSequenceIndex, TagsSequence.Num() - 1,
		*TagsSequence[CurrentSequenceIndex].ToString());

	bWaitingForInput = true;
	GetWorld()->GetTimerManager().SetTimer(
		InputTimerHandle,
		this,
		&APortalBase::HandleInputTimeout,
		TimeLimitPerInput,
		false
	);
}

void APortalBase::AddPlayerToGame(APrototypeCharacter* Player)
{
	if (!HasAuthority())
	{
		UE_LOG(LogPortalBase, Warning, TEXT("Portal [%s] - Attempted to add player on client"), *GetName());
		return;
	}

	if (!Player)
	{
		UE_LOG(LogPortalBase, Error, TEXT("Portal [%s] - Attempted to add null player"), *GetName());
		return;
	}

	if (CharactersPlaying.Contains(Player))
	{
		UE_LOG(LogPortalBase, Warning, TEXT("Portal [%s] - Player [%s] already in game"), *GetName(), *Player->GetName());
		return;
	}

	if (CharactersPlaying.Num() >= MaxPlayers)
	{
		UE_LOG(LogPortalBase, Warning, TEXT("Portal [%s] - Cannot add player: Max players reached (%d)"), *GetName(), MaxPlayers);
		return;
	}

	CharactersPlaying.Add(Player);
	GrantPortalAbilitiesToPlayer(Player);
	
	// Añadir el nuevo jugador al mapa de completado
	if (bIsSequenceActive)
	{
		PlayersCompletedCurrentStep.Add(Player, false);
	}

	UE_LOG(LogPortalBase, Log, TEXT("Portal [%s] - Added player [%s] to game. Total players: %d"), 
		*GetName(), *Player->GetName(), CharactersPlaying.Num());
}

void APortalBase::RemovePlayerFromGame(APrototypeCharacter* Player)
{
	if (!HasAuthority())
	{
		UE_LOG(LogPortalBase, Warning, TEXT("Portal [%s] - Attempted to remove player on client"), *GetName());
		return;
	}

	if (!Player)
	{
		UE_LOG(LogPortalBase, Error, TEXT("Portal [%s] - Attempted to remove null player"), *GetName());
		return;
	}

	if (CharactersPlaying.Remove(Player) > 0)
	{
		RevokePortalAbilitiesFromPlayer(Player);
		
		// Remover al jugador del mapa de completado
		PlayersCompletedCurrentStep.Remove(Player);
		
		// Si el jugador estaba en medio de una secuencia, verificar si los demás pueden continuar
		if (bIsSequenceActive)
		{
			CheckAndAdvanceSequence();
		}

		UE_LOG(LogPortalBase, Log, TEXT("Portal [%s] - Removed player [%s] from game. Players remaining: %d"),
			*GetName(), *Player->GetName(), CharactersPlaying.Num());

		if (CharactersPlaying.Num() == 0)
		{
			UE_LOG(LogPortalBase, Log, TEXT("Portal [%s] - Last player left, stopping sequence"), *GetName());
			StopPortalSequence();
		}
	}
	else
	{
		UE_LOG(LogPortalBase, Warning, TEXT("Portal [%s] - Attempted to remove player [%s] that wasn't in the game"),
			*GetName(), *Player->GetName());
	}
}

void APortalBase::OnRep_IsSequenceActive()
{
	UE_LOG(LogPortalBase, Verbose, TEXT("Portal [%s] - Sequence state replicated: %s"),
		*GetName(), bIsSequenceActive ? TEXT("Active") : TEXT("Inactive"));
}

void APortalBase::HandleInputTimeout()
{
	if (!HasAuthority() || !bIsSequenceActive)
	{
		UE_LOG(LogPortalBase, Verbose, TEXT("Portal [%s] - Input timeout ignored: Authority=%s, Active=%s"),
			*GetName(),
			HasAuthority() ? TEXT("Yes") : TEXT("No"),
			bIsSequenceActive ? TEXT("Yes") : TEXT("No"));
		return;
	}

	UE_LOG(LogPortalBase, Log, TEXT("Portal [%s] - Input timeout at sequence index %d"), *GetName(), CurrentSequenceIndex);
	StopPortalSequence();
	OnSequenceCompleted.Broadcast(false);
}

void APortalBase::GrantPortalAbilitiesToPlayer(APrototypeCharacter* Player)
{
	if (!Player)
	{
		UE_LOG(LogPortalBase, Error, TEXT("Portal [%s] - Attempted to grant abilities to null player"), *GetName());
		return;
	}

	APlayerState* PlayerState = Player->GetPlayerState();
	APlayerStateBase* PSBase = Cast<APlayerStateBase>(PlayerState);
	
	if (!PSBase)
	{
		UE_LOG(LogPortalBase, Error, TEXT("Portal [%s] - Failed to get valid PlayerStateBase for player [%s]"),
			*GetName(), *Player->GetName());
		return;
	}

	for (auto AbilityClass : PortalAbilities)
	{
		if (AbilityClass)
		{
			FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
			PSBase->GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
			UE_LOG(LogPortalBase, Log, TEXT("Portal [%s] - Granted ability [%s] to player [%s]"),
				*GetName(), *AbilityClass->GetName(), *Player->GetName());
		}
	}
}

void APortalBase::RevokePortalAbilitiesFromPlayer(APrototypeCharacter* Player)
{
	if (!Player)
	{
		UE_LOG(LogPortalBase, Error, TEXT("Portal [%s] - Attempted to revoke abilities from null player"), *GetName());
		return;
	}

	APlayerState* PlayerState = Player->GetPlayerState();
	APlayerStateBase* PSBase = Cast<APlayerStateBase>(PlayerState);

	if (!PSBase)
	{
		UE_LOG(LogPortalBase, Error, TEXT("Portal [%s] - Failed to get valid PlayerStateBase for player [%s]"),
			*GetName(), *Player->GetName());
		return;
	}

	for (auto AbilityClass : PortalAbilities)
	{
		if (AbilityClass)
		{
			PSBase->GetAbilitySystemComponent()->ClearAbility(FGameplayAbilitySpecHandle());
			UE_LOG(LogPortalBase, Log, TEXT("Portal [%s] - Revoked ability [%s] from player [%s]"),
				*GetName(), *AbilityClass->GetName(), *Player->GetName());
		}
	}
}


