// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/PrototypeCharacter.h"

#include "AbilitySystemComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "AbilitySystem/PrototypeAbilitySet.h"
#include "AbilitySystem/AbilitySystemComponent/CustomAbilitySystemComponent.h"
#include "Characters/PrototypeCharacterMovementComponent.h"
#include "Equipment/PrototypeEquipmentManagerComponent.h"
#include "Equipment/PrototypeQuickBarComponent.h"
#include "Input/CustomInputComponent.h"
#include "Inventory/PTInventoryManagerComponent.h"
#include "Player/PlayerStateBase.h"


//////////////////////////////////////////////////////////////////////////
// APrototypeCharacter

APrototypeCharacter::APrototypeCharacter(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPrototypeCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	// GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	

	EquipmentManager = CreateDefaultSubobject<UPrototypeEquipmentManagerComponent>("EquipmentManager");

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
	bReplicateUsingRegisteredSubObjectList = true;
}
void APrototypeCharacter::InitializeAbilitySystem()
{
	APlayerStateBase* PlayerStateBase = GetPlayerState<APlayerStateBase>();
	if (PlayerStateBase)
	{
		UAbilitySystemComponent* ASC = PlayerStateBase->GetAbilitySystemComponent();
		AbilitySystemComponent = Cast<UCustomAbilitySystemComponent>(ASC);
		AbilitySystemComponent->InitAbilityActorInfo(PlayerStateBase, this);
		
		// Initialize AttributeSet reference
		AttributeSetBase = PlayerStateBase->GetAttributeSetBase();
	}
}

void APrototypeCharacter::InitializeAttributes()
{
	Super::InitializeAttributes();
}


void APrototypeCharacter::AddCharacterAbilities()
{
	Super::AddCharacterAbilities();
}

void APrototypeCharacter::AbilityInputTagPressed(FGameplayTag GameplayTag)
{
	
	AbilitySystemComponent->AbilityInputTagPressed(GameplayTag);	
}

void APrototypeCharacter::AbilityInputTagReleased(FGameplayTag GameplayTag)
{
	AbilitySystemComponent->AbilityInputTagReleased(GameplayTag);
}

void APrototypeCharacter::AbilityInputTagHeld(FGameplayTag GameplayTag)
{
	AbilitySystemComponent->AbilityInputTagHeld(GameplayTag);
}

void APrototypeCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Initialize Ability System for server
	InitializeAbilitySystem();
	AddCharacterAbilities();
	InitializeAttributes();
	AddStartupEffects();
	

}

void APrototypeCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// Initialize Ability system for client
	InitializeAbilitySystem();
	InitializeAttributes();
	
}

//////////////////////////////////////////////////////////////////////////
// Input

void APrototypeCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APrototypeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UCustomInputComponent* EnhancedInputComponent = Cast<UCustomInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APrototypeCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APrototypeCharacter::Look);
		EnhancedInputComponent->BindAbilityActions(InputActionToGameplayTag, this, &APrototypeCharacter::AbilityInputTagPressed, &APrototypeCharacter::AbilityInputTagReleased, &APrototypeCharacter::AbilityInputTagHeld);
	}

	
}

void APrototypeCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APrototypeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
