// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	//UItemWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized();

	UFUNCTION()
	void OnClickedUseButton();

	UFUNCTION()
	FText GetToolTipUseButton();

public:

	UPROPERTY(BlueprintReadWrite, Category = "Item")
	class AItem* Item;

	TWeakObjectPtr<class UInventoryComponent> Inventory;

	UPROPERTY(meta = (BindWidget))
	class UButton* UseButton;

	UPROPERTY(meta = (BindWidget))
	class UImage* Thumbnail;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemName;

	void SetItemInfo(class AItem* PickItem);
	

};
