// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()

public:

	ALobbyHUD();

	virtual void BeginPlay() override;

	void SetPlayerNum(int32 PlayerNum);


public:


	UPROPERTY()
	class ULobbyUserWidget* LobbyWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = true))
	TSubclassOf<class ULobbyUserWidget> LobbyWidgetClass;
	
};
