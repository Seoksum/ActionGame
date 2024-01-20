// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameworks/ActionGameInstance.h"
#include "Algo/Transform.h"
#include "Kismet/GameplayStatics.h"


UActionGameInstance::UActionGameInstance()
{
	// ������ ���̺� �޾ƿ��� 
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
	UWorld* World = GetWorld(); // ������ ���� ������ ������

	if (World)
	{
		UE_LOG(LogTemp,Log,TEXT("MYLOG MaxLevel : %d"), MaxLevel);
		World->ServerTravel("/Game/Maps/LobbyMap?listen");
		//�������� .  ������ �Է��� ����� ������ �̵���Ŵ, Ŭ���̾�Ʈ�� ���̰� ������ �������� Ŭ���̾�Ʈ����
		//�÷��̾� ��Ʈ�ѷ����� ClientTravel�� ȣ����.
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
