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
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;


public:

	APooledObject_Sphere();
	
	UPROPERTY(EditDefaultsOnly, Category = "Bot")
	float BaseDamage;

	

	// Bullet Bot 
public:
	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AEnemy> EnemyClass;

public:

	UPROPERTY(VisibleDefaultsOnly)
	class AEnemy* Enemy;

	void FindEnemyPath();

	FVector GetNextPathPoint();

	FVector NextPathPoint;


	UPROPERTY(EditDefaultsOnly, Category = "Bot")
	bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "Bot")
	float ForceValue;

	UPROPERTY(EditDefaultsOnly, Category = "Bot")
	bool AttackComplete;




};
