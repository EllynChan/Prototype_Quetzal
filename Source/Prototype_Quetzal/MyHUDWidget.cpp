// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUDWidget.h"
#include "Components/ProgressBar.h"

void UMyHUDWidget::SetHealthPercent(float HealthPercent)
{
    if (HealthBar)
    {
        HealthBar->SetPercent(HealthPercent);
    }
}

void UMyHUDWidget::SetStaminaPercent(float StaminaPercent)
{
    if (StaminaBar)
    {
        StaminaBar->SetPercent(StaminaPercent);
    }
}
