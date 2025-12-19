// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyHUDWidget.h"
#include "Prototype_QuetzalCharacter.h"
#include "Prototype_QuetzalPlayerController.generated.h"

class UInputMappingContext;
class UUserWidget;

/**
 *  Basic PlayerController class for a third person game
 *  Manages input mappings
 */
UCLASS(abstract)
class APrototype_QuetzalPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category ="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> MobileExcludedMappingContexts;

	/** Mobile controls widget to spawn */
	UPROPERTY(EditAnywhere, Category="Input|Touch Controls")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	/** Pointer to the mobile controls widget */
	TObjectPtr<UUserWidget> MobileControlsWidget;

	UPROPERTY(EditAnywhere, Category = "UI_HUD")
	TSubclassOf<UMyHUDWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "UI_HUD")
	TObjectPtr<UMyHUDWidget> HUDWidget;

	/** Character class to respawn when the possessed pawn is destroyed */
	UPROPERTY(EditAnywhere, Category = "Respawn")
	TSubclassOf<APrototype_QuetzalCharacter> CharacterClass;

	/** Transform to respawn the character at. Can be set to create checkpoints */
	FTransform RespawnTransform;

	UFUNCTION()
	void OnPawnHealthChanged(float HealthPercent);

	void OnPossess(APawn* InPawn) override;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

	/** Called if the possessed pawn is destroyed */
	UFUNCTION()
	void OnPawnDestroyed(AActor* DestroyedActor);

public:
	/** Updates the character respawn transform */
	void SetRespawnTransform(const FTransform& NewRespawn);
};
