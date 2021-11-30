// Copyright Epic Games, Inc. All Rights Reserved.

#include "MiniGameJamGameMode.h"
#include "MiniGameJamHUD.h"
#include "Player/CharacterBase.h"
#include "UObject/ConstructorHelpers.h"

AMiniGameJamGameMode::AMiniGameJamGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/_Blueprint/Player/BP_CharacterBase.BP_CharacterBase_C"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AMiniGameJamHUD::StaticClass();
}
