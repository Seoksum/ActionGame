// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Enemy.h"
#include "Enemy_ACorpse.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AEnemy_ACorpse : public AEnemy
{
	GENERATED_BODY()

public:

	AEnemy_ACorpse();


protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:

	virtual void Attack() override;

	UFUNCTION()
	void AttackCheck();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class UDamageType> DamageType;
	
};
