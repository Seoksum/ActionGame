// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/ActionGameHUD.h"
#include "Characters/ActionGameCharacter.h"
#include "GameData/GameEnum.h"
#include "Interface/InGameWidgetInterface.h"
#include "ActionPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AActionPlayerController : public APlayerController , public IInGameWidgetInterface
{
	GENERATED_BODY()

public:
	AActionPlayerController();

	virtual void BeginPlay() override;

    virtual void OnPossess(APawn* aPawn) override;


	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetPlayerPlay();

	UFUNCTION(Reliable, Client)
	void ClientHUDStateChanged(EHUDState NewState);
	void ClientHUDStateChanged_Implementation(EHUDState NewState);
    
    FORCEINLINE UClass* GetPlayerPawnClass() { return MyPawnClass; }

protected:

    /* Return The Correct Pawn Class Client-Side */
    UFUNCTION(Reliable, Client)
    void DeterminePawnClass();
    virtual void DeterminePawnClass_Implementation();

    /* Set Pawn Class On Server For This Controller */
    UFUNCTION(Reliable, Server, WithValidation)
    virtual void ServerSetPawn(TSubclassOf<class AActionGameCharacter> InPawnClass);
    virtual void ServerSetPawn_Implementation(TSubclassOf<class AActionGameCharacter> InPawnClass);
    virtual bool ServerSetPawn_Validate(TSubclassOf<class AActionGameCharacter> InPawnClass);


protected:
    /* Actual Pawn class we want to use */
    UPROPERTY(Replicated)
    TSubclassOf<class AActionGameCharacter> MyPawnClass;

    /* First Pawn Type To Use */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Controller")
    TSubclassOf<class AActionGameCharacter> PawnToUseA;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Controller")
    TSubclassOf<class AActionGameCharacter> PawnToUseB;


public:

    uint8 HUDState;

    //void ChangeHUDState(EHUDState NewState);
    UFUNCTION(Reliable, Client)
    void ChangeHUDState(EHUDState NewState);
    virtual void ChangeHUDState_Implementation(EHUDState NewState);

    bool ApplyHUD(UUserWidget* WidgetToApply, bool bShowMouseCursor, bool EnableClickEvents); // screen에 hud를 적용한 후 성공여부 반환

    void RemoveHUD(EHUDState NewState);


    virtual void AddInGameWidget(class UInGameWidget* InGame) override;

    virtual void AddSkillWidget(class USkillWidget* InSkillWidget) override;

    void SetInGameLevel(float InLevel);

    bool bOnInventoryHUD;

    void SetPlayerNum(int32 PlayerNum);

    UFUNCTION(Reliable, Client)
    void SetLoadingScreen();
    virtual void SetLoadingScreen_Implementation();

    virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;
   


public:

    ECharacterType CharaterType;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class USkillWidget* SkillWidget;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UInGameWidget* InGameWidget;

    UPROPERTY()
    class UUserWidget* InventoryWidget;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> InventoryWidgetClass;

    UPROPERTY()
    class ULobbyUserWidget* LobbyWidget;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = true))
    TSubclassOf<class ULobbyUserWidget> LobbyWidgetClass;

    UPROPERTY()
    UUserWidget* LoadingScreenWidget;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> LoadingScreenWidgetClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
    UUserWidget* WinScreenWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    TSubclassOf<UUserWidget> WinScreenWidgetClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
    UUserWidget* LoseScreenWidget;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    TSubclassOf<UUserWidget> LoseScreenWidgetClass;




};


