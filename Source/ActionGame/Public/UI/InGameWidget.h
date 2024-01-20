// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UInGameWidget : public UPlayerUserWidget
{
	GENERATED_BODY()
	
public:

	void BindStat(class UStatComponent* StatComp);
	void SetLevel(int32 PlayerLevel);
	void SetCharacterSkill(int32 CharacterType);


	
protected:

	virtual void NativeConstruct() override;


public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Level;

	UPROPERTY(meta = (BindWidget))
	class USkillWidget* WBP_SkillWidget;


private:

	TWeakObjectPtr<class UStatComponent> CurrentStatComp;

};
