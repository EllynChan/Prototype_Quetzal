// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UInputAction;
struct FInputActionValue;

UCLASS(Abstract)
class PROTOTYPE_QUETZAL_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Cleanup */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MouseLookAction;

	FVector2D CurrMoveInput;

	/** Max amount of HP the character will have on respawn */
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (ClampMin = 0, ClampMax = 100))
	float MaxHP = 100.0f;

	/** Current amount of HP the character has */
	UPROPERTY(VisibleAnywhere, Category = "Damage")
	float CurrentHP = 0.0f;

	/** Time to wait before respawning the character */
	UPROPERTY(EditAnywhere, Category = "Respawn", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float RespawnTime = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float BaseAttack = 25.0f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float BaseDefense = 10.0f;

	/** Character respawn timer */
	FTimerHandle RespawnTimer;

	/** Copy of the mesh's transform so we can reset it after ragdoll animations */
	FTransform MeshStartingTransform;

public:
	// Sets default values for this character's properties
	AMyCharacter();

	/** Resets the character's current HP to maximum */
	void ResetHP();

	/** Handles death events */
	UFUNCTION(BlueprintCallable, Category = "Custom")
	virtual void HandleDeath();

	/** Handles healing events */
	virtual void ApplyHealing(float Healing, AActor* Healer);

	/** Called from the respawn timer to destroy and re-create the character */
	void RespawnCharacter();

	/** Overrides the default TakeDamage functionality */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:	
	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();

	UFUNCTION(BlueprintCallable, Category = "Input")
	bool IsMoving() const { return GetPendingMovementInputVector().IsNearlyZero(); }

	UFUNCTION(BlueprintPure, Category = "Damage")
	FORCEINLINE float GetCurrentHP() const { return CurrentHP; }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);
	FOnHealthChanged OnHealthChanged;

	UFUNCTION(BlueprintPure, Category = "Damage")
	FORCEINLINE float GetMaxHP() const { return MaxHP; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
