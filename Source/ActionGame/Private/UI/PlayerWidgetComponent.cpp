// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerWidgetComponent.h"
#include "UI/PlayerUserWidget.h"

void UPlayerWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UPlayerUserWidget* PlayerUserWidget = Cast<UPlayerUserWidget>(GetWidget());
	if (PlayerUserWidget)
	{
		PlayerUserWidget->SetOwningActor(GetOwner());
	}
}