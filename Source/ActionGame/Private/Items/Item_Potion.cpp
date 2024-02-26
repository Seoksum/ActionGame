// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item_Potion.h"
#include "Interface/PlayerStateInterface.h"
#include "Components/StatComponent.h"
#include "Components/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"



AItem_Potion::AItem_Potion()
{
	HpPotionVal = 15.f;
	ManaPotionVal = 15.f;
}

void AItem_Potion::BeginPlay()
{
	Super::BeginPlay();

	//CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AItem_Potion::OnBeginOverlap);
}

void AItem_Potion::Use()
{
	Super::Use();

	IPlayerStateInterface* Player = Cast<IPlayerStateInterface>(GetOwner());
	if (Player)
	{
		UStatComponent* Stat = Player->GetMyStatComponent();
		if (Stat)
		{
			if (ItemName.EqualTo(FText::FromString("HpPotion")))
			{
				Stat->DrinkHpPotion(HpPotionVal); // HpPotionVal = 15.f;
			}
			else if (ItemName.EqualTo(FText::FromString("ManaPotion")))
			{
				Stat->DrinkManaPotion(ManaPotionVal); // ManaPotionVal = 15.f;
			}
		}
	}
}
