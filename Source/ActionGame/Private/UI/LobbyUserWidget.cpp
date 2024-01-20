// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyUserWidget.h"
#include "Components/TextBlock.h"


void ULobbyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	

}

void ULobbyUserWidget::SetPlayerNum(int32 PlayerNum)
{
	const FString PlayerNumText = FString::Printf(TEXT("%d"), PlayerNum);
	NumOfPlayer->SetText(FText::FromString(PlayerNumText));
}
