// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ActionGameCharacter.h"
#include "ActionGameCharacter_Knight.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AActionGameCharacter_Knight : public AActionGameCharacter
{
	GENERATED_BODY()
	
public:

	AActionGameCharacter_Knight();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



protected:

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;

	

	//virtual void Attack() override;
	void SkillAttack(float Damage, float TraceDistance, float Radius);
	void SkillUltimateAttack(float Damage, float TraceDistance, float Radius);
	

protected:

	UPROPERTY()
	class AWeapon_Sword* SwordWeapon;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AWeapon_Sword> SwordClass;

	




};
