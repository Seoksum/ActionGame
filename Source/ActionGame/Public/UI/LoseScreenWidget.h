// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoseScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API ULoseScreenWidget : public UUserWidget
{
	GENERATED_BODY()


protected:

	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void ClickRespawnBtn();


private:
	UPROPERTY(meta = (BindWIdget))
	class UButton* RespawnBtn;


	
};
