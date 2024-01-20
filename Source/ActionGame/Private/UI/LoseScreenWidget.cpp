// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoseScreenWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/ActionPlayerController.h"
#include "GameFrameworks/ActionGameModeBase.h"
#include "Characters/ActionGameCharacter.h"
#include "Interface/PlayerStateInterface.h"


void ULoseScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void ULoseScreenWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RespawnBtn->OnClicked.AddDynamic(this, &ULoseScreenWidget::ClickRespawnBtn);
	
}

void ULoseScreenWidget::ClickRespawnBtn()
{
	//AActionPlayerController* PlayerController = Cast< AActionPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	AActionPlayerController* PlayerController = Cast<AActionPlayerController>(GetOwningPlayer());
	
	if (PlayerController)
	{
		PlayerController->RemoveHUD(EHUDState::LoseScreen);
		AActionGameCharacter* Character = Cast<AActionGameCharacter>(GetOwningPlayerPawn());
		Character->RespawnCharacter();
	}

	RemoveFromParent();
}
