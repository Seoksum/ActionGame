// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Interface/GameModeInterface.h"
#include "ActionGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AActionGameModeBase : public AGameMode,public IGameModeInterface
{
	GENERATED_BODY()

public:
	AActionGameModeBase();

	virtual void BeginPlay() override;
	
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	virtual void PostLogin(class APlayerController* NewPlayer) override;

	virtual void IncreaseExp(AController* KillerController, float Exp) override;
	virtual void OnEnemyKilled() override;

	FTimerHandle TimerHandle_BossMap;
	void TravelToBossMap();
	void EndGame(bool bIsPlayerWinner);


	virtual void Logout(AController* Exiting) override;


	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	int32 ClearScore;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
	int32 CurrentScore;



};
