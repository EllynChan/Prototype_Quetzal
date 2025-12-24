// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Prototype_Quetzal.h"
#include "MyHUDWidget.h"
#include "MyCharacter.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FlushPressedKeys();

	HUDWidget = CreateWidget<UMyHUDWidget>(this, HUDWidgetClass);
	DeathScreenWidget = CreateWidget<UUserWidget>(this, DeathScreenWidgetClass);

	if (HUDWidget)
	{
		HUDWidget->AddToPlayerScreen(0);
	}
	else
	{
		UE_LOG(LogPrototype_Quetzal, Error, TEXT("Could not spawn HUD widget."));
	}
}

void AMyPlayerController::SetRespawnTransform(const FTransform& NewRespawn)
{
	// save the new respawn transform
	RespawnTransform = NewRespawn;
}

void AMyPlayerController::OnPawnHealthChanged(float HealthPercent)
{
	if (HUDWidget)
	{
		HUDWidget->SetHealthPercent(HealthPercent);
	}
}

void AMyPlayerController::ShowDeathScreen()
{
	if (DeathScreenWidget)
	{
		DeathScreenWidget->AddToViewport();

		SetInputMode(FInputModeUIOnly());
	}
}

void AMyPlayerController::HideDeathScreen()
{
	if (DeathScreenWidget)
	{
		DeathScreenWidget->RemoveFromParent();
		DeathScreenWidget = nullptr;

		SetInputMode(FInputModeGameOnly());
	}
}

void AMyPlayerController::BindToCharacter(AMyCharacter* InCharacter)
{
	if (!InCharacter) return;

	InCharacter->OnHealthChanged.AddUObject(this, &AMyPlayerController::OnPawnHealthChanged);
	InCharacter->OnDeath.AddUObject(this, &AMyPlayerController::ShowDeathScreen);
	InCharacter->OnRespawn.AddUObject(this, &AMyPlayerController::HideDeathScreen);
}
