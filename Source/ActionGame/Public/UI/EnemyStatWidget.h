// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "EnemyStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UEnemyStatWidget : public UPlayerUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


public:

	void BindHp(class UStatComponent* StatComp);
	void UpdateHp(float CurrentHp, float MaxHp);


private:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HpBar;

	TWeakObjectPtr<class UStatComponent> CurrentStatComp;
	
};
