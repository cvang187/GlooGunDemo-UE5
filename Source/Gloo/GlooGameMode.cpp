// Copyright Epic Games, Inc. All Rights Reserved.

#include "GlooGameMode.h"
#include "GlooCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGlooGameMode::AGlooGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
