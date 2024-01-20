// Copyright Epic Games, Inc. All Rights Reserved.

#include "ActionGameGameMode.h"
#include "Characters/ActionGameCharacter.h"
#include "UObject/ConstructorHelpers.h"


AActionGameGameMode::AActionGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}
