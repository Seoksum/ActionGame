// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StartGameWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetSwitcher.h"
#include "GameFrameworks/ActionGameInstance.h"
#include "UI/InputIPWidget.h"
#include "GameData/GameEnum.h"
#include "GameFrameworks/StartGamePlayerController.h"
#include "UObject/UObjectGlobals.h"


void UStartGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->bShowMouseCursor = true;

	WBP_InputIP = CreateWidget<UInputIPWidget>(GetWorld(), WBP_InputIPClass);
	WBP_InputIP->RemoveFromViewport();

	MyGameInstance = Cast<UActionGameInstance>(GetGameInstance());

	WeaponComboBox->AddOption(FString("Knight"));
	WeaponComboBox->AddOption(FString("Maze"));
	WeaponComboBox->OnSelectionChanged.AddDynamic(this, &UStartGameWidget::SelectWeaponType);

	Btn_CreateServer->OnClicked.AddDynamic(this, &UStartGameWidget::CreateServer);
	Btn_JoinServer->OnClicked.AddDynamic(this, &UStartGameWidget::JoinServer);

}

void UStartGameWidget::CreateServer()
{
	const FString& Addr = "/Game/Maps/LobbyMap?listen";
	AStartGamePlayerController* PlayerController = Cast<AStartGamePlayerController>(GetOwningPlayer());
	if (PlayerController)
	{
		PlayerController->ASyncLevelLoad(Addr, true);
	}

	

	//MyGameInstance->Host();
}

void UStartGameWidget::JoinServer()
{
	this->RemoveFromParent();
	WBP_InputIP->AddToViewport();

}

void UStartGameWidget::SelectWeaponType(FString sItem, ESelectInfo::Type type)
{

	if (sItem.Equals("Knight"))
	{
		MyGameInstance->SetCharacterType(ECharacterType::Knight);
	}
	else if (sItem.Equals("Maze"))
	{
		MyGameInstance->SetCharacterType(ECharacterType::Maze);
	}
}