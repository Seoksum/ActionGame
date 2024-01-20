// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyHUD.h"
#include "UI/LobbyUserWidget.h"

ALobbyHUD::ALobbyHUD()
{
	static ConstructorHelpers::FClassFinder<ULobbyUserWidget> LobbyClassRef(TEXT("WidgetBlueprint'/Game/UI/Loading/WBP_LobbyWidget.WBP_LobbyWidget_C'"));
	if (LobbyClassRef.Succeeded())
	{
		LobbyWidgetClass = LobbyClassRef.Class;
		LobbyWidget = CreateWidget<ULobbyUserWidget>(GetWorld(), LobbyWidgetClass);
	}


}

void ALobbyHUD::BeginPlay()
{
	if (LobbyWidget)
	{
		LobbyWidget->AddToViewport();
	}
}

void ALobbyHUD::SetPlayerNum(int32 PlayerNum)
{
	if (LobbyWidget)
	{
		LobbyWidget->SetPlayerNum(PlayerNum);
	}
}

