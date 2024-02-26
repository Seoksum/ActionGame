// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWidget.h"
//#include "Characters/ActionGameCharacter.h"
#include "Components/InventoryComponent.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "UI/ItemWidget.h"
#include "Items/Item.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/PlayerStateInterface.h"


void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	IPlayerStateInterface* Player = Cast<IPlayerStateInterface>(GetOwningPlayerPawn());
	if (Player)
	{
		Inventory = Player->GetMyInventoryComponent();
		if (Inventory)
		{
			Inventory->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::RefreshInventory);
		}
	}
}

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnClickedCloseButton);
}

void UInventoryWidget::RefreshInventory()
{
	InventoryBox->ClearChildren(); // �ߺ� ���� ������ �����ϱ� ���� �켱 Clear ���ݴϴ�. 
	TArray<class AItem*> CurrentItems = Inventory->GetItems();

	for (auto& Item : CurrentItems)
	{
		if (Item)
		{
			auto ItemWidget = CreateWidget<UItemWidget>(GetOwningPlayer(), ItemHUDClass);
			if (ItemWidget)
			{
				InventoryBox->AddChildToWrapBox(ItemWidget);
				ItemWidget->SetItemInfo(Item);
			}
		}
	}
}

void UInventoryWidget::OnClickedCloseButton()
{
	RemoveFromParent();
	GetOwningPlayer()->SetShowMouseCursor(false);
}
