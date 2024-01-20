// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterStat.h"
#include "MyGameSingleton.generated.h"

/**
 * 
 */
UCLASS()
class UMyGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UMyGameSingleton();

	static UMyGameSingleton& Get();

	// Character Stat Table 
	FCharacterStat GetCharacterStat(int32 Level);

	UPROPERTY()
	int32 MaxLevel; // 레벨이 몇개있는지

	FORCEINLINE TArray<FCharacterStat> GetCharacterStatTable() const { return CharacterStatTable; }


	

private:

	TArray<FCharacterStat> CharacterStatTable;
};
