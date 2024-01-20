// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "StatWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UStatWidget : public UPlayerUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	

public:

	void BindStatComponent(class UStatComponent* StatComp);

	void UpdateHp(float CurrentHp,float MaxHp);
	void UpdateMana(float CurrentMana,float MaxMana);



private:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HpBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_ManaBar;

	UPROPERTY()
	TWeakObjectPtr<class UStatComponent> CurrentStatComp;


};
