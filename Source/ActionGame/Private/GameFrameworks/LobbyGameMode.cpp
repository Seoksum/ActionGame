// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameworks/LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameData/GameEnum.h"
#include "Characters/ActionPlayerController.h"
#include "GameFrameworks/ActionGameState.h"
#include "EngineUtils.h"


ALobbyGameMode::ALobbyGameMode()
{
	PlayerControllerClass = AActionPlayerController::StaticClass();
	GameStateClass = AActionGameState::StaticClass();

	
}

void ALobbyGameMode::BeginPlay()
{
}

UClass* ALobbyGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	AActionPlayerController* MyController = Cast<AActionPlayerController>(InController);
	if (IsValid(MyController))
	{
		return MyController->GetPlayerPawnClass();
	}

	return DefaultPawnClass;
}

void ALobbyGameMode::TravelToBattleMap()
{
	GetWorldTimerManager().SetTimer(GameTimerHandle, this, &ALobbyGameMode::DefaultGameTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}

void ALobbyGameMode::DefaultGameTimer()
{
	AActionGameState* const MyGameState = Cast<AActionGameState>(GameState);

	if (MyGameState && MyGameState->RemainingTime > 0)
	{
		MyGameState->RemainingTime--;
		if (MyGameState->RemainingTime <= 0)
		{
			MyGameState->SetLoadingScreen();
			GetWorld()->ServerTravel(TEXT("/Game/Maps/BattleMap?listen"));
			GetWorldTimerManager().ClearTimer(GameTimerHandle);
		}
	}
}