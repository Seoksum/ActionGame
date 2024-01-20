// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameData/GameEnum.h"
#include "ActionGameHUD.generated.h"

/**
 * 
 */

//UENUM(BlueprintType)
//enum class EHUDState : uint8
//{
//	Playing,
//	Spectating,
//	MatchEnd,
//	Inventory,	// �κ��丮 UI
//};

UCLASS()
class ACTIONGAME_API AActionGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AActionGameHUD();

	virtual void BeginPlay() override;

	// �߰� 
	class UInGameWidget* GetInGameWidget();

	EHUDState CurrentState;

	ECharacterType CharaterType;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	EHUDState GetCurrentState() const;

	void HUDStateChanged(EHUDState NewState);

	void RemoveAllWidgets();
	
	void SetPlayerNum(int32 PlayerNum);






protected:

	UPROPERTY()
	class UInGameWidget* InGame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = true))
	TSubclassOf<class UInGameWidget> InGameHUDClass;

	UPROPERTY()
	class ULobbyUserWidget* LobbyWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = true))
	TSubclassOf<class ULobbyUserWidget> LobbyWidgetClass;

	
};
