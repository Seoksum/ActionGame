// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Enemy.h"
#include "Enemy_Corpse.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AEnemy_Corpse : public AEnemy
{
	GENERATED_BODY()
	

public:

	AEnemy_Corpse();

	
protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:

	virtual void Attack() override;

	UFUNCTION()
	void AttackCheck();

	//UFUNCTION(Reliable, Server)
	//	void ServerAttackCheck();


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class UDamageType> DamageType;

};
