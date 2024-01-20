// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Item_Potion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AItem_Potion : public AItem
{
	GENERATED_BODY()
	
public:

	AItem_Potion();

	virtual void BeginPlay() override;

	virtual void Use() override;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	float HpPotionVal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	float ManaPotionVal;


};
