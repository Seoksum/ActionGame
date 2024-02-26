// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ActionPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/GameModeBase.h"
#include "Characters/ActionGameCharacter.h"
#include "GameFrameworks/ActionGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GameData/GameEnum.h"
#include "UI/InGameWidget.h"
#include "UI/InventoryWidget.h"
#include "UI/SkillWidget.h"
#include "UI/LobbyUserWidget.h"
#include "../ActionGame.h"
#include "EngineUtils.h"


AActionPlayerController::AActionPlayerController()
{
	static ConstructorHelpers::FClassFinder<UInventoryWidget> Inventory_Class(TEXT("WidgetBlueprint'/Game/UI/Inventory/WBP_Inventory.WBP_Inventory_C'"));
	if (Inventory_Class.Succeeded())
	{
		InventoryWidgetClass = Inventory_Class.Class;
		InventoryWidget = CreateWidget(GetWorld(), InventoryWidgetClass);
	}

	static ConstructorHelpers::FClassFinder<ULobbyUserWidget> LobbyClassRef(TEXT("WidgetBlueprint'/Game/UI/Loading/WBP_LobbyWidget.WBP_LobbyWidget_C'"));
	if (LobbyClassRef.Succeeded())
	{
		LobbyWidgetClass = LobbyClassRef.Class;
		LobbyWidget = CreateWidget<ULobbyUserWidget>(GetWorld(), LobbyWidgetClass);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> LoadingScreenClassRef(TEXT("WidgetBlueprint'/Game/UI/Loading/WBP_LoadingScreen.WBP_LoadingScreen_C'"));
	if (LoadingScreenClassRef.Succeeded())
	{
		LoadingScreenWidgetClass = LoadingScreenClassRef.Class;
		LoadingScreenWidget = CreateWidget(GetWorld(), LoadingScreenWidgetClass);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> LoseClassRef(TEXT("WidgetBlueprint'/Game/UI/InGame/WBP_Lose.WBP_Lose_C'"));
	if (LoseClassRef.Succeeded())
	{
		LoseScreenWidgetClass = LoseClassRef.Class;
		//LoseScreenWidget = CreateWidget(GetWorld(), LoseScreenWidgetClass);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> WinClassRef(TEXT("WidgetBlueprint'/Game/UI/InGame/WBP_Win.WBP_Win_C'"));
	if (WinClassRef.Succeeded())
	{
		WinScreenWidgetClass = WinClassRef.Class;
		WinScreenWidget = CreateWidget(GetWorld(), WinScreenWidgetClass);
	}

	static ConstructorHelpers::FClassFinder<AActionGameCharacter> BP_KnightCharacter(TEXT("Blueprint'/Game/Blueprints/Character/BP_MurielKnight.BP_MurielKnight_C'"));
	static ConstructorHelpers::FClassFinder<AActionGameCharacter> BP_MazeCharacter(TEXT("Blueprint'/Game/Blueprints/Character/BP_MurielMaze.BP_MurielMaze_C'"));

	//PawnToUseA = BP_KnightCharacter.Class;
	//PawnToUseB = BP_MazeCharacter.Class;
	
	PawnToUseA = BP_MazeCharacter.Class;
	PawnToUseB = BP_KnightCharacter.Class;

	bOnInventoryHUD = false;
	bReplicates = true;

	if (PlayerCameraManager)
	{
		PlayerCameraManager->bShouldSendClientSideCameraUpdate = true;
		PlayerCameraManager->bUseClientSideCameraUpdates = true;
	}
}

void AActionPlayerController::BeginPlay()
{
	NET_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	Super::BeginPlay();


	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());

	DeterminePawnClass();

	SetPlayerPlay();
}

void AActionPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

}

void AActionPlayerController::SetPlayerPlay()
{
	if (!HasAuthority())
	{
		return;
	}

	PlayerState->SetIsSpectator(false);
	ChangeState(NAME_Playing);
	bPlayerIsWaiting = false;

	// 클라이언트에게 상태 업데이트
	ClientGotoState(NAME_Playing);
	ClientHUDStateChanged(EHUDState::Playing);
}

void AActionPlayerController::ClientHUDStateChanged_Implementation(EHUDState NewState)
{
	AActionGameHUD* HUD = GetHUD<AActionGameHUD>();
	if (HUD)
	{
		HUD->HUDStateChanged(NewState);
	}
}

// Pawn Class
void AActionPlayerController::DeterminePawnClass_Implementation()
{
	if (IsLocalController()) //Only Do This Locally (NOT Client-Only, since Server wants this too!)
	{
		UActionGameInstance* MyGameInstance = Cast<UActionGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (MyGameInstance)
		{
			int32 CharacterType = MyGameInstance->GetCharacterType();
			if (CharacterType == ECharacterType::Knight)
			{
				ServerSetPawn(PawnToUseA);
				CharaterType = ECharacterType::Knight;
			}
			else if (CharacterType == ECharacterType::Maze)
			{
				ServerSetPawn(PawnToUseB);
				CharaterType = ECharacterType::Maze;
			}
		}
	}
}

bool AActionPlayerController::ServerSetPawn_Validate(TSubclassOf<class AActionGameCharacter> InPawnClass)
{
	return true;
}

void AActionPlayerController::ServerSetPawn_Implementation(TSubclassOf<class AActionGameCharacter> InPawnClass)
{
	//MyPawnClass = Cast< AActionGameCharacter>(InPawnClass->StaticClass());
	MyPawnClass = InPawnClass;
	/* Just in case we didn't get the PawnClass on the Server in time... */
	GetWorld()->GetAuthGameMode()->RestartPlayer(this);

}

void AActionPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (bIsWinner)
	{
		ChangeHUDState(EHUDState::WinScreen);
	}
	else
	{
		if (IsLocalController())
		{
			LoseScreenWidget = CreateWidget(GetWorld(), LoseScreenWidgetClass);
			ChangeHUDState(EHUDState::LoseScreen);
		}
	}
}

void AActionPlayerController::ChangeHUDState_Implementation(EHUDState NewState)
{
	EHUDState State = NewState;

	switch (State)
	{
	case EHUDState::Inventory:
	{
		ApplyHUD(InventoryWidget, true, true);
		bOnInventoryHUD = true;
		break;
	}
	case EHUDState::LoadingScreen:
	{
		ApplyHUD(LoadingScreenWidget, true, true);
		break;
	}

	case EHUDState::WinScreen:
	{
		ApplyHUD(WinScreenWidget, true, true);
		break;
	}

	case EHUDState::LoseScreen:
	{
		ApplyHUD(LoseScreenWidget, true, true);
		break;
	}

	default:
	{
		break;
	}
	}
}

bool AActionPlayerController::ApplyHUD(UUserWidget* WidgetToApply, bool IsShowMouseCursor, bool IsEnableClickEvents)
{
	//적용하기전 위젯 체크
	if (WidgetToApply)
	{
		bShowMouseCursor = IsShowMouseCursor;
		bEnableClickEvents = IsEnableClickEvents;

		WidgetToApply->AddToViewport();

		return true;
	}
	return false;
}

void AActionPlayerController::RemoveHUD(EHUDState NewState)
{
	UUserWidget* NewWidget = nullptr;
	switch (NewState)
	{
	case EHUDState::Inventory:
		NewWidget = InventoryWidget;
		bOnInventoryHUD = false;
		bShowMouseCursor = false;
		bEnableClickEvents = false;
		break;

	case EHUDState::LoseScreen:
		NewWidget = LoseScreenWidget;
		bShowMouseCursor = false;
		bEnableClickEvents = false;
		break;
		
	default:
		break;
	}

	if (NewWidget)
	{
		NewWidget->RemoveFromParent();
	}

}

void AActionPlayerController::AddInGameWidget(class UInGameWidget* InGame)
{
	if (IsLocalController())
	{
		InGameWidget = InGame;
	}
}

void AActionPlayerController::AddSkillWidget(class USkillWidget* InSkillWidget)
{
	if (IsLocalController())
	{
		SkillWidget = InSkillWidget;
	}
}

void AActionPlayerController::SetInGameLevel(float InLevel)
{
	if (InGameWidget)
	{
		InGameWidget->SetLevel(static_cast<int32>(InLevel));
	}
}

void AActionPlayerController::SetPlayerNum(int32 PlayerNum)
{
	if (LobbyWidget)
	{
		LobbyWidget->SetPlayerNum(PlayerNum);
	}
}

void AActionPlayerController::SetLoadingScreen_Implementation()
{
	ChangeHUDState(EHUDState::LoadingScreen);
}


// Replication
void AActionPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AActionPlayerController, MyPawnClass);

}

