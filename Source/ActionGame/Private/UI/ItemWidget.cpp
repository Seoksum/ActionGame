// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/InventoryComponent.h"
#include "Characters/ActionGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Item.h"


void UItemWidget::NativeConstruct()
{
	Super::NativeConstruct();


}

void UItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UseButton->OnClicked.AddDynamic(this, &UItemWidget::OnClickedUseButton); // 아이템 버튼 클릭
	UseButton->ToolTipTextDelegate.BindUFunction(this, FName("GetToolTipUseButton")); // 아이템에 마우스를 갖다대면 설명서 표시
}

void UItemWidget::OnClickedUseButton()
{
	AActionGameCharacter* Player = Cast<AActionGameCharacter>(GetOwningPlayerPawn());
	if (Player)
	{
		Player->UseItem(Item); // 각 아이템에 맞는 효과가 발생합니다. 
	}
}

FText UItemWidget::GetToolTipUseButton()
{
	// 아이템의 기능을 알려줍니다.
	return (("Click to {0} {1},{2}"), Item->UseActionText, Item->ItemName, Item->ItemDescription);
}

void UItemWidget::SetItemInfo(class AItem* PickItem)
{
	// 아이템의 썸네일과 이름을 표시합니다.  
	Item = PickItem;

	UTexture2D* MyThumbnail = Item->Thumnail;
	if (MyThumbnail)
		Thumbnail->SetBrushFromTexture(MyThumbnail);

	FText Name = Item->ItemName;
	ItemName->SetText(Name);
}

