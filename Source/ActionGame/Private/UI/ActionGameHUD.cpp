// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ActionGameHUD.h"
#include "Blueprint/UserWidget.h"
#include "TextureResource.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/InGameWidget.h"
#include "UI/InventoryWidget.h"
#include "UI/LobbyUserWidget.h"
#include "Characters/ActionPlayerController.h"
#include "GameData/GameEnum.h"
#include "GameFrameworks/ActionGameInstance.h"
#include "Kismet/GameplayStatics.h"


AActionGameHUD::AActionGameHUD()
{
	static ConstructorHelpers::FClassFinder<UInGameWidget> InGameClass(TEXT("WidgetBlueprint'/Game/UI/InGame/WBP_InGame.WBP_InGame_C'"));
	if (InGameClass.Succeeded())
	{
		InGameHUDClass = InGameClass.Class;
	}

	static ConstructorHelpers::FClassFinder<ULobbyUserWidget> LobbyClassRef(TEXT("WidgetBlueprint'/Game/UI/Loading/WBP_LobbyWidget.WBP_LobbyWidget_C'"));
	if (LobbyClassRef.Succeeded())
	{
		LobbyWidgetClass = LobbyClassRef.Class;
	}


}

void AActionGameHUD::BeginPlay()
{
	AActionPlayerController* PlayerController = Cast<AActionPlayerController>(GetOwningPlayerController());

	if (PlayerController)
	{
		CharaterType = PlayerController->CharaterType;
	}

	InGame = CreateWidget<UInGameWidget>(GetWorld(), InGameHUDClass);
	InGame->SetCharacterSkill(CharaterType);
	InGame->AddToViewport();

}

EHUDState AActionGameHUD::GetCurrentState() const
{
	return CurrentState;
}

void AActionGameHUD::HUDStateChanged(EHUDState NewState)
{
	//RemoveAllWidgets();

	CurrentState = NewState;
	UUserWidget* NewWidget = nullptr;
	switch (NewState)
	{
	case EHUDState::Playing:
		NewWidget = InGame;
		break;
	default:
		break;
	}

	if (NewWidget)
	{
		NewWidget->AddToViewport();
	}
}

void AActionGameHUD::RemoveAllWidgets()
{
	TArray<UUserWidget*> RemoveWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), RemoveWidgets, UUserWidget::StaticClass());

	for (UUserWidget* Widget : RemoveWidgets)
	{
		Widget->RemoveFromParent();
	}
}

void AActionGameHUD::SetPlayerNum(int32 PlayerNum)
{
	if (LobbyWidget)
	{
		LobbyWidget->SetPlayerNum(PlayerNum);
	}
}


UInGameWidget* AActionGameHUD::GetInGameWidget()
{
	if (InGame)
	{
		return InGame;
	}
	return nullptr;
}
