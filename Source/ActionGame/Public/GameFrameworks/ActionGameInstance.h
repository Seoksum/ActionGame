// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameData/CharacterStat.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "ActionGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FServerInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FString ServerName;

	UPROPERTY(BlueprintReadOnly)
	FString PlayerCountStr;

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayers;

	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayers;

	UPROPERTY(BlueprintReadOnly)
		int32 ServerArrayIndex;

	void SetPlayerCount()
	{
		PlayerCountStr = FString(FString::FromInt(CurrentPlayers) + "/" + FString::FromInt(MaxPlayers));
	}

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerDel, FServerInfo, ServerListDel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerSearchingDel, bool, SearchingForServer);

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
	void Host(); //호스트

	UFUNCTION(Exec) //조인 + 접속아이피
	void Join(const FString& Address);

	void SetCharacterType(int32 InCharacterType);
	int32 GetCharacterType();

	FCharacterStat GetCharacterStat(int32 Level);

	FORCEINLINE TArray<FCharacterStat> GetCharacterStatTable() const { return CharacterStatTable; }
	
	UPROPERTY()
	int32 MaxLevel; // 레벨이 몇개있는지

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


	// OnlineSubsystem
protected:

	FName MySessionName;

	UPROPERTY(BlueprintAssignable)
	FServerDel ServerListDel;

	UPROPERTY(BlueprintAssignable)
	FServerSearchingDel SearchingForServerDel;

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	virtual void Init() override;

	virtual void OnCreateSessionComplete(FName SessionName, bool Succeeded);
	virtual void OnFindSessionsComplete(bool Succeeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);


	UFUNCTION(BlueprintCallable)
		void CreateServer(FString ServerName, FString HostName);

	UFUNCTION(BlueprintCallable)
		void FindServers();

	UFUNCTION(BlueprintCallable)
	void JoinServer(int32 ArrayIndex);

};
