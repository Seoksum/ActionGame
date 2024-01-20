// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ActionGameCharacter.h"
#include "ActionGameCharacter_Maze.generated.h"

/**
 * 
 */

//USTRUCT()
//struct FHitScanTrace
//{
//	GENERATED_BODY()
//
//public:
//
//	UPROPERTY()
//	FHitResult Hit;
//
//	UPROPERTY()
//	FVector_NetQuantize TraceTo;
//};

UCLASS()
class ACTIONGAME_API AActionGameCharacter_Maze : public AActionGameCharacter
{
	GENERATED_BODY()
	

public:

	AActionGameCharacter_Maze();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//UPROPERTY(ReplicatedUsing = OnRep_SocketLocation)
	UPROPERTY(Replicated)
	FVector SocketLocation;

	UPROPERTY(Replicated)
	FVector ShotDirection;

	//UFUNCTION()
	//void OnRep_SocketLocation();

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
	void SkillUltimate_Maze(float Damage, float TraceDistance, float Radius);


protected:

	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	class UObjectPool* ObjectPool;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(EditAnywhere, Category = "Skill")
	class UParticleSystem* Particle;

	UPROPERTY(EditAnywhere, Category = "Skill")
	class UParticleSystem* AttackParticle;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float HealAmount;



};
