// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ActionGameState.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AActionGameState : public AGameState
{
	GENERATED_BODY()
	
public:

	AActionGameState();

	virtual void HandleBeginPlay() override;

	virtual void OnRep_ReplicatedHasBegunPlay() override;


public:
	virtual void AddPlayerState(APlayerState* PlayerState);
	virtual void RemovePlayerState(APlayerState* PlayerState);

	void OnReadyToStart();

	void EndGame();

	void SetWinScreen();
	void SetLoadingScreen();

public:

	UPROPERTY(Transient, Replicated)
	int32 RemainingTime;

protected:

	int32 MaxNumOfPlayers;




};
