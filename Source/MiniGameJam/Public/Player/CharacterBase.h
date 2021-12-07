// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterBase.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

UCLASS()
class MINIGAMEJAM_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
		USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh, BlueprintReadOnly)
		USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh, BlueprintReadOnly)
		USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AMiniGameJamProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		uint8 bUsingMotionControllers : 1;


private:

	void Run();

	void Walk();

	UPROPERTY(EditAnyWhere)
		int MaxRun = 1000;

	UPROPERTY(EditAnyWhere)
		int MaxWalk = 600;

	bool isRun = false;

	UCharacterMovementComponent* Movement = GetCharacterMovement();

	bool Move = false;

	UPROPERTY(EditAnyWhere)
		float UpCharge = 25;

	UPROPERTY(EditAnyWhere)
		float DownCharge = 25;

	UPROPERTY(EditAnyWhere)
		float UpResistence = 25;

	UPROPERTY(EditAnyWhere)
		float DownResistence = 25;

	UPROPERTY(EditAnyWhere)
		int fDamage = 50;

	bool ChargeHot = false;

public:

	UFUNCTION(BlueprintPure)
		float GetHealthPercent() const;

	UFUNCTION(BlueprintPure)
		float GetResistencePercent() const;

	UFUNCTION(BlueprintPure)
		float GetChangePercent() const;

	UFUNCTION(BlueprintPure)
		int GetCountKill() const;

	UFUNCTION(BlueprintPure)
		int GetNowRound() const;

	UFUNCTION(BlueprintPure)
		bool GetFire() const;

	UFUNCTION(BlueprintPure)
		bool GetHot() const;

	UFUNCTION(BlueprintPure)
		bool BossBox() const;

	UFUNCTION(BlueprintPure)
		bool RoundBox() const;

	UPROPERTY(EditAnyWhere)
		float Health;

	UPROPERTY(EditAnyWhere)
		float Resistence;

	UPROPERTY(EditAnyWhere)
		float Charge;

	UPROPERTY(EditAnyWhere)
		int MaxHealth = 100;

	UPROPERTY(EditAnyWhere)
		int MaxResistence = 100;

	UPROPERTY(EditAnyWhere)
		int MaxCharge = 100;

	bool Fire = false;

	UPROPERTY(EditAnyWhere)
		int CountKill = 0;

	UPROPERTY(EditAnyWhere)
		int NowRound = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool BossFight = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		bool BeginBattle = false;

	UFUNCTION(BlueprintCallable)
		void vBeginBattle(bool Begin);

	UFUNCTION(BlueprintCallable)
		void vDamage(float Damage);

	UFUNCTION(BlueprintCallable)
		void vHealth(float fHealth);

	UFUNCTION(BlueprintCallable)
		void vNowRound(int Round);

	UFUNCTION(BlueprintCallable)
		void vKill(int Kill);

	UFUNCTION(BlueprintCallable)
		void vBattleBoss(bool bBattleBoss);

protected:

	/** Fires a projectile. */
	void OnFire();

	void NotFire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);





public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};
