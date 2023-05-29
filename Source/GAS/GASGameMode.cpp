// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASGameMode.h"
#include "GASCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGASGameMode::AGASGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
