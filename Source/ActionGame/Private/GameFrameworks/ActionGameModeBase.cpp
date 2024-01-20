// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameworks/ActionGameModeBase.h"
#include "Characters/ActionGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameData/GameEnum.h"
#include "Characters/ActionPlayerController.h"
#include "GameFrameworks/ActionGameState.h"
#include "AI/AIController_Enemy.h"
#include "EngineUtils.h"

AActionGameModeBase::AActionGameModeBase()
{
	PlayerControllerClass = AActionPlayerController::StaticClass();
	GameStateClass = AActionGameState::StaticClass();

}

void AActionGameModeBase::BeginPlay()
{
	Super::BeginPlay();


}

UClass* AActionGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	AActionPlayerController* MyController = Cast<AActionPlayerController>(InController);
	if (IsValid(MyController))
	{
		return MyController->GetPlayerPawnClass();
	}

	return DefaultPawnClass;
}

void AActionGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);


}

void AActionGameModeBase::IncreaseExp(AController* KillerController, float Exp)
{
	AActionGameCharacter* PlayerCharacter = Cast<AActionGameCharacter>(KillerController->GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->IncreaseExp(Exp);
	}
}

void AActionGameModeBase::OnEnemyKilled()
{
	for (AAIController_Enemy* Controller : TActorRange<AAIController_Enemy>(GetWorld()))
	{
		if (!Controller->IsDead())
		{
			return; // ���� �������� �׾��ִ� AI�� ���ٸ� ���� return 
		}
	}
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		Controller->GameHasEnded(Controller->GetPawn(), true);
	}
}

void AActionGameModeBase::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		Controller->GameHasEnded(Controller->GetPawn(), bIsPlayerWinner);
	}
}

void AActionGameModeBase::TravelToBossMap()
{// ������ �̵�
	AActionGameState* const MyGameState = Cast<AActionGameState>(GameState);
	if (MyGameState)
	{
		MyGameState->SetLoadingScreen();
		GetWorld()->ServerTravel(TEXT("/Game/Maps/BossMap?listen"));
	}
}

void AActionGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

}

