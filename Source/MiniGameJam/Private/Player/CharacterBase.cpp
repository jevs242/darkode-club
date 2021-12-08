// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CharacterBase.h"
#include "MiniGameJam/MiniGameJamProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Enemy.h"


#define print(x) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT(x));
#define log(x) UE_LOG(LogTemp, Error, TEXT(x));

// Sets default values
ACharacterBase::ACharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);


}

// Called when the game starts or when spawned
void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	Resistence = MaxResistence;
	Health = MaxHealth;
	Charge = MaxCharge;
}

void ACharacterBase::NotFire()
{
	Fire = false;
}

// Called every frame
void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isRun && Resistence > 0)
	{
		Resistence -= DownResistence * DeltaTime;
	}
	else if (!isRun && Resistence <= MaxResistence)
	{
		Resistence += UpResistence * DeltaTime;
	}
	else if (Resistence <= 0)
	{
		Walk();
	}

	if (Fire && Charge > 0)
	{
		Charge -= DownCharge * DeltaTime;
	}
	else if (!Fire && Charge <= MaxCharge)
	{
		Charge += UpCharge * DeltaTime;
		if (Charge >= MaxCharge)
		{
			ChargeHot = false;
		}
	}

	if (Charge <= 0)
	{
		Fire = false;
		ChargeHot = true;
	}

	if (Fire)
	{
		FVector Loc;
		FRotator Rot;
		GetController()->GetPlayerViewPoint(Loc,Rot);


		FHitResult* Hitresult = new FHitResult();
		FVector start = Loc;
		FVector End = Loc + (Rot.Vector() * 1500.f);
		FCollisionQueryParams TraceParams;
		FVector Start = FP_Gun->GetComponentLocation();
		//DrawDebugLine(GetWorld(), start, End, FColor::Red, true, 2.f, false, 4.f);

		if (GetWorld()->LineTraceSingleByChannel(*Hitresult, start, End, ECC_Visibility, TraceParams))
		{
			AEnemy* Hit = Cast<AEnemy>(Hitresult->GetActor());
			if (Hit)
			{
				if (!Hit->Shield)
				{
					Hit->Health -= fDamage * DeltaTime;
				}
			}
		}
	}


}

// Called to bind functionality to input
void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACharacterBase::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACharacterBase::NotFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ACharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBase::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACharacterBase::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACharacterBase::LookUpAtRate);

	//Run 

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ACharacterBase::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ACharacterBase::Walk);
}

void ACharacterBase::vBeginBattle(bool Begin)
{
	BeginBattle = Begin;
	NowRound = 1;
}

void ACharacterBase::vDamage(float Damage)
{
	Health -= Damage;
}

void ACharacterBase::vHealth(float fHealth)
{
	Health = fHealth;
}

void ACharacterBase::vNowRound(int SumRound)
{
	if (NowRound != 7)
	{
		NowRound += SumRound;
	}
}

void ACharacterBase::vKill(int Kill)
{
	CountKill = Kill;
}

void ACharacterBase::vBattleBoss(bool bBattleBoss)
{
	BossFight = bBattleBoss;
}

float ACharacterBase::GetHealthPercent() const
{
	return Health / MaxHealth;
}

float ACharacterBase::GetResistencePercent() const
{
	return Resistence / MaxResistence;
}

float ACharacterBase::GetChangePercent() const
{
	return Charge / MaxCharge;
}

int ACharacterBase::GetCountKill() const
{
	return CountKill;
}

bool ACharacterBase::BossBox() const
{
	return BossFight;
}

bool ACharacterBase::RoundBox() const
{
	return BeginBattle;
}

void ACharacterBase::OnFire()
{
	if (Charge > 0 && !ChargeHot || Charge > 100 && ChargeHot)
	{
		Fire = true;




		if (Charge > 100 && ChargeHot)
		{
			ChargeHot = false;
		}
	}


	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		//UWorld* const World = GetWorld();
		//if (World != nullptr)
		//{

		//	const FRotator SpawnRotation = GetControlRotation();
		//	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		//	const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

		//	//Set Spawn Collision Handling Override
		//	FActorSpawnParameters ActorSpawnParams;
		//	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		//	// spawn the projectile at the muzzle
		//	World->SpawnActor<AMiniGameJamProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

		//}
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		//UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		//// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

int ACharacterBase::GetNowRound() const
{
	return NowRound;
}

bool ACharacterBase::GetFire() const
{
	return Fire;
}

bool ACharacterBase::GetHot() const
{
	return ChargeHot;
}

void ACharacterBase::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
		Move = true;
	}
	else
	{
		Move = false;
	}
}

void ACharacterBase::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
		Move = true;
	}
	else
	{
		Move = false;
	}
}

void ACharacterBase::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::Run()
{
	if (GetCharacterMovement()->Velocity.Size() != 0)
	{
		Movement->MaxWalkSpeed = MaxRun;
		isRun = true;
	}
}

void ACharacterBase::Walk()
{
	Movement->MaxWalkSpeed = MaxWalk;
	isRun = false;
}
