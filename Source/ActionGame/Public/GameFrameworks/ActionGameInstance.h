// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameData/CharacterStat.h"
#include "ActionGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UActionGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	UActionGameInstance();


	UFUNCTION(Exec)
	void Host(); //ȣ��Ʈ

	UFUNCTION(Exec) //���� + ���Ӿ�����
	void Join(const FString& Address);

	void SetCharacterType(int32 InCharacterType);
	int32 GetCharacterType();

	FCharacterStat GetCharacterStat(int32 Level);

	FORCEINLINE TArray<FCharacterStat> GetCharacterStatTable() const { return CharacterStatTable; }
	
	UPROPERTY()
	int32 MaxLevel; // ������ ��ִ���

	void SetCurrentLevel(float InCurrentLevel);
	float GetCurrentLevel();

	void SetTravelToMap(bool InTravelToBattelMap);
	bool GetTravelToMap();



private:

	UPROPERTY()
	int32 CharacterType;

	UPROPERTY()
	float CurrentLevel;

	TArray<FCharacterStat> CharacterStatTable;

	UPROPERTY()
	bool bTravelToBattelMap;



};
