// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyHUDWidget.h"
#include "MyCharacter.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_QUETZAL_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	/** Gameplay initialization */
	virtual void BeginPlay() override;

	virtual void BindToCharacter(AMyCharacter* InCharacter);

	UPROPERTY(EditAnywhere, Category = "UI_HUD")
	TSubclassOf<UMyHUDWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI_HUD")
	TObjectPtr<UMyHUDWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> DeathScreenWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TObjectPtr<UUserWidget> DeathScreenWidget;

	/** Transform to respawn the character at. Can be set to create checkpoints */
	FTransform RespawnTransform;

public:
	/** Updates the character respawn transform */
	void SetRespawnTransform(const FTransform& NewRespawn);

	void ShowDeathScreen();

	void HideDeathScreen();
	
	void OnPawnHealthChanged(float HealthPercent);
};
