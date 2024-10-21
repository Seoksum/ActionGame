// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ActionGameCharacter.h"
#include "ActionGameCharacter_Maze.generated.h"

/**
 *
 */


UCLASS()
class ACTIONGAME_API AActionGameCharacter_Maze : public AActionGameCharacter
{
	GENERATED_BODY()


public:

	AActionGameCharacter_Maze();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* SpawnedEffect;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class APooledObject_Sphere* SphereObject;



protected:

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;


	UFUNCTION()
	void DefaultAttack(float InDamage, float InTraceDistance, float InRadius);

	UFUNCTION()
	void SkillQ_Maze();


	UFUNCTION()
	void SkillE_Maze();

	UFUNCTION()
	void SkillUltimate_Maze(float InDamage, float InTraceDistance, float InRadius);



protected:

	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	class UObjectPool* ObjectPool;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(EditAnywhere, Category = "Skill")
	class UParticleSystem* HealParticle;

	UPROPERTY(EditAnywhere, Category = "Skill")
	class UParticleSystem* AttackParticle;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float TraceDistance;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float Radius;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float HealAmount;



};
