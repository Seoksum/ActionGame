// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PooledObject.h"
#include "PooledObject_Sphere.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API APooledObject_Sphere : public APooledObject
{
	GENERATED_BODY()

protected:

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;


public:

	APooledObject_Sphere();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AEnemy> EnemyClass;

	UPROPERTY(EditDefaultsOnly, Category = "Object")
	bool AttackComplete;

protected:

	UPROPERTY(VisibleDefaultsOnly)
	class AEnemy* Enemy;

	void FindEnemyPath();

	FVector GetNextPathPoint();

	FVector NextPathPoint;

	UPROPERTY(EditDefaultsOnly, Category = "Object")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Object")
	bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "Object")
	float ForceSize;

	UPROPERTY(EditAnywhere, Category = "Particle Effect")
	class UParticleSystem* AttackParticle;

	UPROPERTY(EditAnywhere, Category = "Particle Effect")
	class USoundBase* AttackSound;

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayEffect(FTransform SpawnTransform,UParticleSystem* Particle, USoundBase* Sound);






};
