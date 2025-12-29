// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UInputAction;
struct FInputActionValue;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Default,
	Acting,
	Disabled,
	Dead
};

enum class EStatusEffect : uint8
{
	Invincible,
	Stunned,
	Airborne
};

enum class EBasicSkill : uint8
{
	Dash,
	BasicAttack
};

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

	/** Dash Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* DashAction;

	/** Dash Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AttackAction;

	FVector2D CurrMoveInput;

	/** Time to wait before respawning the character */
	UPROPERTY(EditAnywhere, Category = "Respawn", meta = (ClampMin = 0, ClampMax = 10, Units = "s"))
	float RespawnTime = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float BaseAttack = 25.0f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float BaseDefense = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* HitEmitter;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> HitCameraShake;

	// Stats effects
	TSet<EBasicSkill> SkillsOnCD;

	TMap<EBasicSkill, FTimerHandle> CDTimers;

	/** Character respawn timer */
	FTimerHandle RespawnTimer;

	/** Character dashing timer */
	FTimerHandle DashTimerHandle;

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

	UPROPERTY(BlueprintReadOnly, Category = "State")
	ECharacterState CharacterState = ECharacterState::Default;

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for dashing input. effects can be overriden in children */
	virtual void StartDash();

	virtual void EndDash();

	// check if character can perform input actions
	bool CanAct();

	// check if character can take damage
	bool CanTakeDamage();

	// queue and remove basic skill internal cds
	void BeginCD(EBasicSkill skill, float duration);

	void EndCD(EBasicSkill skill);

protected:

	void SpawnHitEffect();

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	FVector HitEmitterOffset = FVector::ZeroVector;

public:	
	/** Max amount of HP the character will have on respawn */
	UPROPERTY(EditAnywhere, Category = "Damage", meta = (ClampMin = 0, ClampMax = 100))
	float MaxHP = 100.0f;

	/** Current amount of HP the character has */
	UPROPERTY(VisibleAnywhere, Category = "Damage")
	float CurrentHP = 0.0f;

	/** Dashing distance */
	UPROPERTY(EditAnywhere, Category = "Input")
	float DashDistance = 400.0f;

	/** Dashing distance */
	UPROPERTY(EditAnywhere, Category = "Input")
	float DashDuration = 0.2f;

	// Stats effects
	TSet<EStatusEffect> ActiveStatusEffects;

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

	bool IsInvincible() const { return ActiveStatusEffects.Contains(EStatusEffect::Invincible); }

	UFUNCTION(BlueprintPure, Category = "Damage")
	FORCEINLINE float GetCurrentHP() const { return CurrentHP; }

	UFUNCTION(BlueprintPure, Category = "Damage")
	FORCEINLINE float GetMaxHP() const { return MaxHP; }

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);
	FOnHealthChanged OnHealthChanged;

	DECLARE_MULTICAST_DELEGATE(FOnDeath);
	FOnDeath OnDeath;

	DECLARE_MULTICAST_DELEGATE(FOnRespawn);
	FOnRespawn OnRespawn;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
