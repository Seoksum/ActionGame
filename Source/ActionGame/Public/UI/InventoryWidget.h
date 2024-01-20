// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized();

	UFUNCTION()
	void RefreshInventory();

	UFUNCTION()
	virtual void OnClickedCloseButton();


public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* CloseButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UWrapBox* InventoryBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* Inventory;

	UPROPERTY(BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = true))
	TSubclassOf<class UItemWidget> ItemHUDClass;

};
