// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API ULobbyUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void SetPlayerNum(int32 PlayerNum);

	void SetCountdown(int32 RemainingCount);



private:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NumOfPlayer;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxNumOfPlayer;

	
};
