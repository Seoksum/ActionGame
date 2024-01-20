// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Interface/LobbyGameModeInterface.h"
#include "LobbyGameMode.generated.h"



/**
 * 
 */
UCLASS()
class ACTIONGAME_API ALobbyGameMode : public AGameMode, public ILobbyGameModeInterface
{
	GENERATED_BODY()
	
public:

	ALobbyGameMode();

	virtual void BeginPlay() override;

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	virtual void TravelToBattleMap() override;

	FTimerHandle GameTimerHandle;

	void DefaultGameTimer();



};
