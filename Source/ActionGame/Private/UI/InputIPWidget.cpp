// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InputIPWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "GameFrameworks/ActionGameInstance.h"
#include "GameFrameworks/StartGamePlayerController.h"

void UInputIPWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	MyGameInstance = Cast<UActionGameInstance>(GetGameInstance());

	Btn_Join->OnClicked.AddDynamic(this, &UInputIPWidget::JoinServer);

}

void UInputIPWidget::JoinServer()
{
	//MyGameInstance->Join(IPAddressField->GetText().ToString());
	
	const FString& Addr = IPAddressField->GetText().ToString();
	AStartGamePlayerController* PlayerController = Cast<AStartGamePlayerController>(GetOwningPlayer());
	if (PlayerController)
	{
		PlayerController->ASyncLevelLoad(Addr, false);
	}
	RemoveFromParent();

}