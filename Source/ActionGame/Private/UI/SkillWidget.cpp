// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "TimerManager.h"
#include "Characters/ActionPlayerController.h"
#include "Interface/InGameWidgetInterface.h"



void USkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IInGameWidgetInterface* PlayerController = Cast<IInGameWidgetInterface>(GetOwningPlayer());
	if (PlayerController)
	{
		PlayerController->AddSkillWidget(this);
	}


}

void USkillWidget::SetKnightSkill()
{

	const FString Q_Path = "Texture2D'/Game/Images/Knight_Q.Knight_Q'";
	const FString E_Path = "Texture2D'/Game/Images/Knight_E.Knight_E'";
	const FString R_Path = "Texture2D'/Game/Images/Knight_R.Knight_R'";

	SetCharacterImage(Q_Path, E_Path, R_Path);
}

void USkillWidget::SetMazeSkill()
{
	const FString Q_Path = "Texture2D'/Game/Images/Maze_Q.Maze_Q'";
	const FString E_Path = "Texture2D'/Game/Images/Maze_E.Maze_E'";
	const FString R_Path = "Texture2D'/Game/Images/Maze_R.Maze_R'";

	SetCharacterImage(Q_Path, E_Path, R_Path);
}

void USkillWidget::SetCharacterImage(FString QPath, FString EPath, FString RPath)
{
	UTexture2D* MyTexture_Q = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *QPath));
	UTexture2D* MyTexture_E = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *EPath));
	UTexture2D* MyTexture_R = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *RPath));

	Image_Q->SetBrushFromTexture(MyTexture_Q);
	Image_E->SetBrushFromTexture(MyTexture_E);
	Image_R->SetBrushFromTexture(MyTexture_R);
}


void USkillWidget::SetSkillCoolTime(int32 InCoolDownTime, char ch)
{
	if (ch == 'Q')
	{
		CoolDownTime_Q = InCoolDownTime;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_SkillCoolTimerQ, this, &USkillWidget::EndSkillCoolTimeQ, 1.f, true);
	}
	else if (ch == 'E')
	{
		CoolDownTime_E = InCoolDownTime;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_SkillCoolTimerE, this, &USkillWidget::EndSkillCoolTimeE, 1.f, true);
	}
	else if (ch=='R')
	{
		CoolDownTime_R = InCoolDownTime;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_SkillCoolTimerR, this, &USkillWidget::EndSkillCoolTimeR, 1.f, true);
	}
}

void USkillWidget::EndSkillCoolTimeQ()
{
	FString Q_Str;

	if (CoolDownTime_Q < 1)
	{
		Q_Str = FString::Printf(TEXT("Q"));
		Text_Q->SetText(FText::FromString(Q_Str));
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_SkillCoolTimerQ);
		return;
	}

	else
	{
		Q_Str = FString::Printf(TEXT("%01d "), CoolDownTime_Q);
	}
	Text_Q->SetText(FText::FromString(Q_Str));
	--CoolDownTime_Q;
}

void USkillWidget::EndSkillCoolTimeE()
{
	FString E_Str;

	if (CoolDownTime_E < 1)
	{
		E_Str = FString::Printf(TEXT("E"));
		Text_E->SetText(FText::FromString(E_Str));
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_SkillCoolTimerE);
		return;
	}

	else
	{
		E_Str = FString::Printf(TEXT("%01d "), CoolDownTime_E);
	}
	Text_E->SetText(FText::FromString(E_Str));
	--CoolDownTime_E;
}

void USkillWidget::EndSkillCoolTimeR()
{
	FString R_Str;

	if (CoolDownTime_R < 1)
	{
		R_Str = FString::Printf(TEXT("R"));
		Text_R->SetText(FText::FromString(R_Str));
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_SkillCoolTimerR);
		return;
	}

	else
	{
		R_Str = FString::Printf(TEXT("%01d "), CoolDownTime_R);
	}
	Text_R->SetText(FText::FromString(R_Str));
	--CoolDownTime_R;
}

