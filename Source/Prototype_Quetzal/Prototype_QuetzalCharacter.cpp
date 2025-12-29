// Copyright Epic Games, Inc. All Rights Reserved.

#include "Prototype_QuetzalCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "InputActionValue.h"
#include "Prototype_Quetzal.h"
#include "Prototype_QuetzalPlayerController.h"

APrototype_QuetzalCharacter::APrototype_QuetzalCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void APrototype_QuetzalCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// update the respawn transform on the Player Controller
	if (APrototype_QuetzalPlayerController* PC = Cast<APrototype_QuetzalPlayerController>(GetController()))
	{
		PC->SetRespawnTransform(GetActorTransform());
	}
}

void APrototype_QuetzalCharacter::HandleDeath()
{
	Super::HandleDeath();

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();

	MoveComp->SetMovementMode(MOVE_Falling);
	MoveComp->GravityScale = 1.0f;
}

void APrototype_QuetzalCharacter::StartDash()
{
	if (!CanAct() || SkillsOnCD.Contains(EBasicSkill::Dash)) return;

	Super::StartDash();

	// Save velocity
	SavedVelocity = GetVelocity();

	// Hide mesh
	GetMesh()->SetVisibility(false);
}

void APrototype_QuetzalCharacter::EndDash()
{
	Super::EndDash();

	// Restore velocity
	LaunchCharacter(SavedVelocity, true, true);

	// Spawn effect
	if (DashEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DashEffect, GetActorLocation());

	// Show mesh
	GetMesh()->SetVisibility(true);
}