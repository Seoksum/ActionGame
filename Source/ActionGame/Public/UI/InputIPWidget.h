// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputIPWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UInputIPWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized();

	UFUNCTION()
	virtual void JoinServer();

private:

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IPAddressField;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Join;

	UPROPERTY()
	class UActionGameInstance* MyGameInstance;

};
