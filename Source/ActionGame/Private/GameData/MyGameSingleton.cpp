// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/MyGameSingleton.h"
#include "Algo/Transform.h"


UMyGameSingleton::UMyGameSingleton()
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

}

UMyGameSingleton& UMyGameSingleton::Get()
{
	UMyGameSingleton* Singleton = CastChecked<UMyGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	return *NewObject<UMyGameSingleton>();
}

FCharacterStat UMyGameSingleton::GetCharacterStat(int32 Level)
{
	if (Level >= 0 && Level <= MaxLevel)
	{
		return CharacterStatTable[Level];
	}
	return FCharacterStat();
}


