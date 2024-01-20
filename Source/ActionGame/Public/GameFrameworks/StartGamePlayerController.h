// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "StartGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AStartGamePlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	AStartGamePlayerController();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ASyncLevelLoad(const FString& Addr, bool IsServer);


private:

	void AsyncLevelLoadFinished(const FString Addr, bool IsServer);

	void AddToLoadingScreen();



public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UStartGameWidget> StartClass;

	class UStartGameWidget* StartWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> LoadingScreenClass;

	class UUserWidget* LoadingScreenWidget;


};
