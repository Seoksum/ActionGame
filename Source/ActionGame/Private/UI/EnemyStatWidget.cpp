// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyStatWidget.h"
#include "Components/StatComponent.h"
#include "Components/ProgressBar.h"
#include "Interface/CharacterWidgetInterface.h"

void UEnemyStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ICharacterWidgetInterface* CharacterWidget = Cast<ICharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);

	}
}

void UEnemyStatWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CurrentStatComp.IsValid())
	{
		//PB_HpBar->SetPercent(CurrentStatComp->GetHpRatio());
	}
}

void UEnemyStatWidget::BindHp(class UStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	CurrentStatComp->OnHpChanged.AddUObject(this, &UEnemyStatWidget::UpdateHp);
}

void UEnemyStatWidget::UpdateHp(float CurrentHp, float MaxHp)
{
	PB_HpBar->SetPercent(CurrentHp / MaxHp);
}
