// Copyright Epic Games, Inc. All Rights Reserved.

#include "GlimmeringTrailGameMode.h"
#include "GlimmeringTrailCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGlimmeringTrailGameMode::AGlimmeringTrailGameMode()
{
	// set default pawn class to our Blueprinted character

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BP_BasePlayer"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
