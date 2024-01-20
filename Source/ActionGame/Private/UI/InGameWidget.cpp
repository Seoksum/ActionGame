// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameWidget.h"
#include "UI/SkillWidget.h"
#include "Components/StatComponent.h"
#include "Interface/InGameWidgetInterface.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "GameData/GameEnum.h"
#include "Characters/ActionGameCharacter.h"
#include "Interface/InGameWidgetInterface.h"


void UInGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IInGameWidgetInterface* PlayerController = Cast<IInGameWidgetInterface>(GetOwningPlayer());
	if (PlayerController)
	{
		PlayerController->AddInGameWidget(this);
	}
}

void UInGameWidget::BindStat(UStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	//CurrentStatComp->OnPlayerLevelUp.AddUObject(this, &UInGameWidget::SetLevel);
}

void UInGameWidget::SetLevel(int32 PlayerLevel)
{
	const FString MyLevel = FString::Printf(TEXT("%d"), PlayerLevel);
	Text_Level->SetText(FText::FromString(MyLevel));
}

void UInGameWidget::SetCharacterSkill(int32 CharacterType)
{
	switch (CharacterType)
	{
	case ECharacterType::Knight:
		WBP_SkillWidget->SetKnightSkill();
		break;

	case ECharacterType::Maze:
		WBP_SkillWidget->SetMazeSkill();
		break;

	default:
		break;
	}
}

