// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledObject.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledObjectDespawn, APooledObject*, PoolActor);

UCLASS()
class ACTIONGAME_API APooledObject : public AActor
{
	GENERATED_BODY()


public:
	// Sets default values for this actor's properties
	APooledObject();

	FOnPooledObjectDespawn OnPooledObjectDespawn;

	UFUNCTION(BlueprintCallable, Category = "Pooled Object")
		void Deactivate();

	void SetActive(bool IsActive);

	void SetPoolIndex(int32 Index);

	bool IsActive();
	int32 GetPoolIndex();

	void AddForceToBullet(FVector Dir);



protected:

	UPROPERTY(ReplicatedUsing = OnRep_SetActiveBullet)
	bool Active;

	UFUNCTION()
	void OnRep_SetActiveBullet();

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	float LifeSpan;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	int32 PoolIndex;

	FTimerHandle LifeSpanTimer;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly,Category="Bullet")
	float ForceSize;



};
