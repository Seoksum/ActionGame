// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Items/Item.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	Capacity = 20.f;

}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UInventoryComponent::AddItem(AItem* Item)
{
	if (ItemArray.Num() >= Capacity || !Item)
		return;

	Item->OwingInventory = this;
	ItemArray.Add(Item);
	OnInventoryUpdated.Broadcast(); // UI 업데이트
}

void UInventoryComponent::RemoveItem(AItem* Item)
{
	if (Item)
	{
		Item->OwingInventory = nullptr;
		ItemArray.RemoveSingle(Item);
		OnInventoryUpdated.Broadcast(); // UI 업데이트
	}
}

TArray<class AItem*> UInventoryComponent::GetItems()
{
	return ItemArray;
}

