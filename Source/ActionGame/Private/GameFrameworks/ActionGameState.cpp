// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameworks/ActionGameState.h"
#include "Characters/ActionPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/ActionPlayerState.h"
#include "EngineUtils.h"
#include "../ActionGame.h"
#include "Net/UnrealNetwork.h"
#include "Interface/LobbyGameModeInterface.h"
#include "UI/LobbyHUD.h"


AActionGameState::AActionGameState()
{

	MaxNumOfPlayers = 2;
	RemainingTime = 3;
	
}


void AActionGameState::HandleBeginPlay()
{
	NET_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::HandleBeginPlay();

	NET_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AActionGameState::OnRep_ReplicatedHasBegunPlay()
{
	NET_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::OnRep_ReplicatedHasBegunPlay();

	NET_LOG(LogNetwork, Log, TEXT("%s"), TEXT("End"));
}

void AActionGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);

	AActionPlayerState* PS = Cast<AActionPlayerState>(PlayerState);
	if (PS)
	{
		PlayerArray.AddUnique(PS);
	}

	for (ALobbyHUD* HUD : TActorRange<ALobbyHUD>(GetWorld()))
	{
		HUD->SetPlayerNum(PlayerArray.Num());
	}
	
	if (PlayerArray.Num() == MaxNumOfPlayers)
	{
		OnReadyToStart();
	}
}

void AActionGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);

	PlayerArray.Remove(PlayerState);

	//if (PlayerArray.Num() == 1)
	//{
	//	EndGame();
	//}
}

void AActionGameState::OnReadyToStart()
{
	ILobbyGameModeInterface* GameMode = GetWorld()->GetAuthGameMode<ILobbyGameModeInterface>();
	if (GameMode)
	{
		GameMode->TravelToBattleMap();
	}
}

void AActionGameState::SetWinScreen()
{
	for (AActionPlayerController* Controller : TActorRange<AActionPlayerController>(GetWorld()))
	{
		//Controller->SetLoadingScreen();
		Controller->ChangeHUDState(EHUDState::WinScreen); 
	}
}

void AActionGameState::SetLoadingScreen()
{
	for (AActionPlayerController* Controller : TActorRange<AActionPlayerController>(GetWorld()))
	{
		//Controller->SetLoadingScreen();
		Controller->ChangeHUDState(EHUDState::LoadingScreen);
	}
}

void AActionGameState::EndGame()
{
	for (AActionPlayerController* Controller : TActorRange<AActionPlayerController>(GetWorld()))
	{
		Controller->ClientHUDStateChanged(EHUDState::MatchEnd);
		DisableInput(Controller);
	}
}

//void AActionGameState::DefaultGameTimer()
//{
//	if (RemainingTime > 0)
//	{
//		RemainingTime--;
//		if (RemainingTime <= 0)
//		{
//			//GetWorld()->ServerTravel(TEXT("/Game/Maps/BattleMap?listen"));
//
//			
//		}
//	}
//}


void AActionGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AActionGameState, RemainingTime);
	

}