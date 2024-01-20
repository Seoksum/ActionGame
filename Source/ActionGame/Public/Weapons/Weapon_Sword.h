// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Weapon.h"
#include "Weapon_Sword.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AWeapon_Sword : public AWeapon
{
	GENERATED_BODY()
	
public:
	AWeapon_Sword();

	void SwordAttack(float InRadius, float InTraceDistance, float InDamage);
	void SwordAttackUltimate(float InRadius, float InTraceDistance, float InDamage);

protected:

	virtual void BeginPlay() override;

	

public:

	UPROPERTY(EditAnywhere, Category = "Particle Effect")
	class UParticleSystem* Particle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class UDamageType> DamageType;


};
