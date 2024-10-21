// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameworks/ActionGameInstance.h"
#include "Algo/Transform.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"



UActionGameInstance::UActionGameInstance()
{
	// 데이터 테이블 받아오기 
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("DataTable'/Game/Data/CharacterStatTable.CharacterStatTable'"));
	if (DataTable.Object)
	{
		const UDataTable* StatDataTable = DataTable.Object;

		TArray<FCharacterStat*> CharacterStats;
		StatDataTable->GetAllRows<FCharacterStat>(TEXT("GetAllRows"), CharacterStats);
		for (int32 i = 0; i < CharacterStats.Num(); ++i)
		{
			CharacterStatTable.Add(*CharacterStats[i]);
		}
	}

	CurrentLevel = 1;
	MaxLevel = CharacterStatTable.Num();
	bTravelToBattelMap = true;
	MySessionName = FName("My Session");
}

void UActionGameInstance::Init()
{
	Super::Init();

	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get())
	{
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			// 델레게이트 바인딩
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UActionGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UActionGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UActionGameInstance::OnJoinSessionComplete);
		}
	}
}

void UActionGameInstance::OnCreateSessionComplete(FName SessionName, bool Succeeded)
{
	
	if (Succeeded)
	{
		GetWorld()->ServerTravel("/Game/Maps/LobbyMap?listen");
	}
}

void UActionGameInstance::OnFindSessionsComplete(bool Succeeded)
{
	if (!SessionInterface.IsValid() || !Succeeded)
	{
		SearchingForServerDel.Broadcast(false);
		return;
	}

	if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString(TEXT("======== Search Result ========"))); }

	int32 ArrayIndex = -1;

	for (FOnlineSessionSearchResult Result : SessionSearch->SearchResults)
	{
		++ArrayIndex;
		if (!Result.IsValid())
			continue;

		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, FString::Printf(TEXT("Result Index : %d"), ArrayIndex));
		
		FServerInfo Info;
		FString ServerName = "Empty Server Name";
		FString HostName = "Empty Host Name";

		Result.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), ServerName);
		Result.Session.SessionSettings.Get(FName("SERVER_HOSTNAME_KEY"), HostName);


		Info.ServerName = ServerName;
		Info.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
		Info.CurrentPlayers = Info.MaxPlayers - Result.Session.NumOpenPublicConnections;
		Info.ServerArrayIndex = ArrayIndex;
		Info.SetPlayerCount();

		ServerListDel.Broadcast(Info);
	}


	//for (auto Result : SessionSearch->SearchResults)
	//{
	//	FString Id = Result.GetSessionIdStr();
	//	FString User = Result.Session.OwningUserName;
	//	// 찾은 세션의 정보 출력하기
	//	if (GEngine)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, FString::Printf(TEXT("Session ID : %s / Owner : %s"), *Id, *User));
	//	}
	//	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	//	SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), "My Session", Result);
	//	// 영상
	//	//SessionInterface->JoinSession(0, "My Session", SessionSearch->SearchResults[0]);
	//}

}

void UActionGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	FString JoinAddress;
	SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
	if (GEngine){GEngine->AddOnScreenDebugMessage(-1,15.f,FColor::Yellow,FString::Printf(TEXT("Connect Address : %s"), *JoinAddress));}


	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		PlayerController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
	}

}

void UActionGameInstance::CreateServer(FString ServerName, FString HostName)
{
	if (!SessionInterface.IsValid()) { if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Game Session is invailed"))); }return; }
	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr) { SessionInterface->DestroySession(NAME_GameSession); }


	TSharedPtr<FOnlineSessionSettings> SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->bIsLANMatch = false;			// LAN 연결
	SessionSettings->NumPublicConnections = 4;		// 최대 접속 가능 수
	SessionSettings->bAllowJoinInProgress = true;	// Session 진행중에 접속 허용
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bShouldAdvertise = true;		// 현재 세션을 광고할지 (스팀의 다른 플레이어에게 세션 홍보 여부)
	SessionSettings->bUsesPresence = true;			// 현재 지역에 세션 표시
	

	SessionSettings->Set(FName("SERVER_NAME_KEY"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	SessionSettings->Set(FName("SERVER_HOSTNAME_KEY"), HostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, MySessionName, *SessionSettings);

	//const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	//SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), MySessionName, *SessionSettings);

}

void UActionGameInstance::FindServers()
{
	if (!SessionInterface.IsValid()) { if (GEngine) { GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, FString(TEXT("Game Session Interface is invailed"))); }return; }

	SearchingForServerDel.Broadcast(true);

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = false;
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());

	//const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	//SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), SessionSearch.ToSharedRef());
}

void UActionGameInstance::JoinServer(int32 ArrayIndex)
{
	FOnlineSessionSearchResult Result = SessionSearch->SearchResults[ArrayIndex];
	if (Result.IsValid())
	{
		//const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
		//SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), MySessionName, Result);

		SessionInterface->JoinSession(0, MySessionName, Result);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString(
			TEXT("Failed to join server at index : %d"), ArrayIndex));
	}
}

void UActionGameInstance::Host()
{
	UWorld* World = GetWorld(); // 레벨의 현재 레벨을 가져옴

	if (World)
	{
		UE_LOG(LogTemp, Log, TEXT("MYLOG MaxLevel : %d"), MaxLevel);
		World->ServerTravel("/Game/Maps/LobbyMap?listen");
		//서버전용 .  서버를 입력한 경로의 맵으로 이동시킴, 클라이언트랑 같이감 서버가 접속중인 클라이언트들의
		//플레이어 컨트롤러에서 ClientTravel을 호출함.
	}

}

void UActionGameInstance::Join(const FString& Address)
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UActionGameInstance::SetCharacterType(int32 InCharacterType)
{
	CharacterType = InCharacterType;
}

int32 UActionGameInstance::GetCharacterType()
{
	return CharacterType;
}

FCharacterStat UActionGameInstance::GetCharacterStat(int32 Level)
{
	if (Level >= 1 && Level < MaxLevel)
	{
		return CharacterStatTable[Level - 1];
	}
	return FCharacterStat();
}

void UActionGameInstance::SetCurrentLevel(float InCurrentLevel)
{
	CurrentLevel = InCurrentLevel;
}

float UActionGameInstance::GetCurrentLevel()
{
	return CurrentLevel;
}

void UActionGameInstance::SetTravelToMap(bool InTravelToBattelMap)
{
	bTravelToBattelMap = InTravelToBattelMap;
}

bool UActionGameInstance::GetTravelToMap()
{
	return bTravelToBattelMap;
}
