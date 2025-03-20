// Copyright Epic Games, Inc. All Rights Reserved.

#include "DarkodeGameMode.h"
#include "DarkodeHUD.h"
#include "Player/CharacterBase.h"
#include "UObject/ConstructorHelpers.h"

ADarkodeGameMode::ADarkodeGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/_Blueprint/Player/BP_CharacterBase.BP_CharacterBase_C"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ADarkodeHUD::StaticClass();
}
