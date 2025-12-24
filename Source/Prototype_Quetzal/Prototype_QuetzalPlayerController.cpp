// Copyright Epic Games, Inc. All Rights Reserved.


#include "Prototype_QuetzalPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "Prototype_Quetzal.h"
#include "MyCharacter.h"
#include "MyHUDWidget.h"
#include "Prototype_QuetzalCharacter.h"
#include "Widgets/Input/SVirtualJoystick.h"

void APrototype_QuetzalPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (SVirtualJoystick::ShouldDisplayTouchInterface() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogPrototype_Quetzal, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void APrototype_QuetzalPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!SVirtualJoystick::ShouldDisplayTouchInterface())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
}

void APrototype_QuetzalPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	BindToCharacter(Cast<AMyCharacter>(InPawn));
	InPawn->OnDestroyed.AddDynamic(this, &APrototype_QuetzalPlayerController::OnPawnDestroyed);
}

void APrototype_QuetzalPlayerController::OnPawnDestroyed(AActor* DestroyedActor)
{
	// spawn a new character at the respawn transform
	if (APrototype_QuetzalCharacter* RespawnedCharacter = GetWorld()->SpawnActor<APrototype_QuetzalCharacter>(CharacterClass, RespawnTransform))
	{
		// possess the character
		Possess(RespawnedCharacter);
		RespawnedCharacter->OnHealthChanged.Broadcast(RespawnedCharacter->CurrentHP / RespawnedCharacter->MaxHP);
	}
}
