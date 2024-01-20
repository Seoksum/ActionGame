// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/ActionGameHUD.h"
#include "StartGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UStartGameWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeOnInitialized();

	UFUNCTION()
	virtual void CreateServer();

	UFUNCTION()
	virtual void JoinServer();

	UFUNCTION()
	virtual void SelectWeaponType(FString sItem, ESelectInfo::Type type);


public:

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_CreateServer;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_JoinServer;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<class UInputIPWidget> WBP_InputIPClass;

	UPROPERTY()
	class UInputIPWidget* WBP_InputIP;

	UPROPERTY()
	class UActionGameInstance* MyGameInstance;

	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* WeaponComboBox;


};
