// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameworks/StartGamePlayerController.h"
#include "UI/StartGameWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UObjectGlobals.h"

AStartGamePlayerController::AStartGamePlayerController()
{
	static ConstructorHelpers::FClassFinder<UStartGameWidget> StartClassRef(TEXT("WidgetBlueprint'/Game/UI/Loading/WBP_StartWidget.WBP_StartWidget_C'"));
	if (StartClassRef.Succeeded())
	{
		StartClass = StartClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> LoadingScreenClassRef(TEXT("WidgetBlueprint'/Game/UI/Loading/WBP_LoadingScreen.WBP_LoadingScreen_C'"));
	if (LoadingScreenClassRef.Succeeded())
	{
		LoadingScreenClass = LoadingScreenClassRef.Class;
		LoadingScreenWidget = CreateWidget(GetWorld(), LoadingScreenClass);
	}

}

void AStartGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (StartClass)
	{
		StartWidget = CreateWidget<UStartGameWidget>(GetWorld(), StartClass);
		//StartWidget->AddToViewport();
	}
	
	if (IsLocalController())
	{
		StartWidget->AddToViewport();
	}
	
	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());
}

void AStartGamePlayerController::AddToLoadingScreen()
{
	if (LoadingScreenClass)
	{
		LoadingScreenWidget = CreateWidget(GetWorld(), LoadingScreenClass);
		LoadingScreenWidget->AddToViewport();
	}
}

void AStartGamePlayerController::ASyncLevelLoad(const FString& Addr, bool IsServer)
{
	LoadPackageAsync(Addr,
		FLoadPackageAsyncDelegate::CreateLambda([=](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
			{
				AsyncLevelLoadFinished(Addr, IsServer);
			}
	), 0, PKG_ContainsMap);


}

void AStartGamePlayerController::AsyncLevelLoadFinished(const FString Addr, bool IsServer)
{
	AddToLoadingScreen();

	if (IsServer)
	{
		GetWorld()->ServerTravel(Addr);
	}
	else
	{
		ClientTravel(Addr, ETravelType::TRAVEL_Absolute);
	}
}
