// Copyright Epic Games, Inc. All Rights Reserved.


#include "Prototype_QuetzalPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "Prototype_Quetzal.h"
#include "MyCharacter.h"
#include "Widgets/Input/SVirtualJoystick.h"

void APrototype_QuetzalPlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUDWidget = CreateWidget<UUserWidget>(this, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToPlayerScreen(0);
	}
	else
	{
		UE_LOG(LogPrototype_Quetzal, Error, TEXT("Could not spawn HUD widget."));
	}

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

	if (AMyCharacter* MyChar = Cast<AMyCharacter>(InPawn))
	{
		MyChar->OnHealthChanged.AddUObject(
			this,
			&APrototype_QuetzalPlayerController::OnPawnHealthChanged
		);
	}
}

void APrototype_QuetzalPlayerController::OnPawnHealthChanged(float HealthPercent)
{
	if (HUDWidget)
	{
		// need to make the hud class first
		//HUDWidget->SetHealthPercent(HealthPercent);
	}
}
