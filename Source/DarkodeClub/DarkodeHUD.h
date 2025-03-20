// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DarkodeHUD.generated.h"

UCLASS()
class ADarkodeHUD : public AHUD
{
	GENERATED_BODY()

public:
	ADarkodeHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

