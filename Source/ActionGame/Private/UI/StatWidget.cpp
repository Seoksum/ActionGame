// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StatWidget.h"
#include "Components/StatComponent.h"
#include "Components/ProgressBar.h"
#include "Interface/CharacterWidgetInterface.h"

void UStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ICharacterWidgetInterface* CharacterWidget = Cast<ICharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}


void UStatWidget::BindStatComponent(class UStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	CurrentStatComp->OnHpChanged.AddUObject(this, &UStatWidget::UpdateHp);
}

void UStatWidget::UpdateHp(float CurrentHp,float MaxHp)
{
	PB_HpBar->SetPercent(CurrentHp / MaxHp);
}


void UStatWidget::UpdateMana(float CurrentMana, float MaxMana)
{
	PB_ManaBar->SetPercent(CurrentMana / MaxMana);
}
