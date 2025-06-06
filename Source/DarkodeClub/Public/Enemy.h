// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class DARKODECLUB_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
		float GetHealthPercent() const;

	UFUNCTION(BlueprintPure)
		bool GetShield() const;

	void DestroyEnemy();

	UFUNCTION(BlueprintCallable)
		void vDamage(float Damage);

	UFUNCTION(BlueprintCallable)
		void vShield(bool bShield);

	UPROPERTY(EditAnyWhere)
		float Health;

	UPROPERTY(EditAnyWhere)
		int MaxHealth = 1000;

	UPROPERTY(EditAnyWhere)
		bool Shield = false;

	FTimerHandle FDeath;
};
