// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API USkillWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;


public:

	void SetKnightSkill();
	void SetMazeSkill();

	void SetCharacterImage(FString QPath, FString EPath, FString RPath);


	// ½ºÅ³ ÄðÅ¸ÀÓ
public:

	FTimerHandle TimerHandle_SkillCoolTimerQ;
	FTimerHandle TimerHandle_SkillCoolTimerE;
	FTimerHandle TimerHandle_SkillCoolTimerR;

	int32 CoolDownTime_Q;
	int32 CoolDownTime_E;
	int32 CoolDownTime_R;

	void SetSkillCoolTime(int32 InCoolDownTime, char ch);

	void EndSkillCoolTimeQ();
	void EndSkillCoolTimeE();
	void EndSkillCoolTimeR();


public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Q;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_E;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_R;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Q;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_E;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_R;

	
	
};
