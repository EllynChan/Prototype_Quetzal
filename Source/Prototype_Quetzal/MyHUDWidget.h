// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "MyHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROTOTYPE_QUETZAL_API UMyHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // Called by controller to update health bar
    UFUNCTION(BlueprintCallable)
    void SetHealthPercent(float HealthPercent);

    // Called by controller to update stamina bar
    UFUNCTION(BlueprintCallable)
    void SetStaminaPercent(float StaminaPercent);

protected:
    // Bind these in the BP designer
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* StaminaBar;
};
