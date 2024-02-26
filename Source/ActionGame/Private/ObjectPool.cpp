// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool.h"
#include "PooledObject.h"
#include "Weapons/PooledObject_Sphere.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "../ActionGame.h"
#include "Characters/ActionGameCharacter_Maze.h"

// Sets default values for this component's properties
UObjectPool::UObjectPool()
{
	PoolIndex = 0;
	PoolSize = 5;
}

void UObjectPool::BeginPlay()
{
	Super::BeginPlay();

	AActionGameCharacter_Maze* Maze = Cast<AActionGameCharacter_Maze>(GetOwner());
	if (Maze && Maze->HasAuthority())
	{
		if (PooledObjectClass != nullptr)
		{
			for (int32 i = 0; i < PoolSize; i++)
			{
				FTransform Transform = FTransform(FRotator().ZeroRotator, Maze->GetActorLocation());
				APooledObject_Sphere* Sphere = GetWorld()->SpawnActorDeferred<APooledObject_Sphere>(PooledObjectClass, Transform);
				Sphere->SetOwningPawn(Maze);
				Sphere->SetActive(false);
				Sphere->SetPoolIndex(PoolIndex++);
				Sphere->OnPooledObjectDespawn.AddDynamic(this, &UObjectPool::PooledObjectDespawn);
				SpherePool.Add(Sphere);
				UGameplayStatics::FinishSpawningActor(Sphere, Transform);
			}
		}
	}
}

//UE_LOG(LogTemp, Log, TEXT("SphereObject Start Loc : (%f,%f,%f)"), Start.Z, Start.Y, Start.Z);

APooledObject_Sphere* UObjectPool::SpawnPooledObject()
{
	// SpherePool에서 꺼낼 수 있을 때
	for (APooledObject_Sphere* SphereObject : SpherePool)
	{
		if (SphereObject != nullptr && !SphereObject->IsActive())
		{
			SpawnedPoolIndex.Add(SphereObject->GetPoolIndex());
			SphereObject->AttackComplete = false;
			return SphereObject;
		}
	}

	// SpherePool에서 꺼낼 수 없을 때
	if (SpawnedPoolIndex.Num() > 0)
	{
		int32 PooledObjectIndex = SpawnedPoolIndex[0];
		SpawnedPoolIndex.Remove(PooledObjectIndex);
		APooledObject_Sphere* SphereObject = SpherePool[PooledObjectIndex];
		if (SphereObject != nullptr)
		{
			SphereObject->SetActive(false);
			SpawnedPoolIndex.Add(SphereObject->GetPoolIndex());
			SphereObject->AttackComplete = false;
			return SphereObject;
		}
	}
	return nullptr;
}


void UObjectPool::PooledObjectDespawn(APooledObject* PoolActor)
{
	SpawnedPoolIndex.Remove(PoolActor->GetPoolIndex());
}

