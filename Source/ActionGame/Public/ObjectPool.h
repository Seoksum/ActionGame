// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PooledObject.h"
#include "ObjectPool.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONGAME_API UObjectPool : public UActorComponent
{
	GENERATED_BODY()


public:	

	UObjectPool();

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	APooledObject_Sphere* SpawnPooledObject(FVector Start,FVector Dir);


	UPROPERTY(EditAnywhere, Category = "Object Pool")
	TSubclassOf<class APooledObject_Sphere> PooledObjectClass;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	int32 PoolSize = 8;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	float PooledObjectLifeSpan = 0.f;

	UFUNCTION()
	void PooledObjectDespawn(APooledObject* PoolActor);

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	int32 PoolIndex;

	UPROPERTY(EditAnywhere, Category = "Object Pool")
	FTransform SpawnTransform;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Object Pool")
	TArray<APooledObject_Sphere*> BulletPool;

	UPROPERTY(VisibleAnywhere, Category = "Object Pool")
	TArray<int32> SpawnedPoolIndexes;






};
