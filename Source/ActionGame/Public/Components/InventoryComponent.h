// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UInventoryComponent();

	virtual void BeginPlay() override;

public:
	void AddItem(class AItem* Item);	// 인벤토리에 아이템 추가
	void RemoveItem(class AItem* Item);	// 인벤토리에 아이템 제거

	TArray<class AItem*> GetItems();

	// 인벤토리에 아이템UI 변경 
	FOnInventoryUpdated OnInventoryUpdated;

private:
	// 최대 용량
	UPROPERTY(EditDefaultsOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 Capacity;

	// 아이템 배열
	UPROPERTY(VisibleAnywhere, Category = "Items")
	TArray<class AItem*> Items;
		

};
