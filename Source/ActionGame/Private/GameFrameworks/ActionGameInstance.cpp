// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameworks/ActionGameInstance.h"
#include "Algo/Transform.h"
#include "Kismet/GameplayStatics.h"


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

}

void UActionGameInstance::Host()
{
	UWorld* World = GetWorld(); // 레벨의 현재 레벨을 가져옴

	if (World)
	{
		UE_LOG(LogTemp,Log,TEXT("MYLOG MaxLevel : %d"), MaxLevel);
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
		return CharacterStatTable[Level-1];
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
