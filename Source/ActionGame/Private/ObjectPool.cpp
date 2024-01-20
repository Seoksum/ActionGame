// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPool.h"
#include "PooledObject.h"
#include "Weapons/PooledObject_Sphere.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "../ActionGame.h"


// Sets default values for this component's properties
UObjectPool::UObjectPool()
{
	PoolIndex = 0;
	PoolSize = 5;

	//SpawnTransform = FTransform(FRotator().ZeroRotator, FVector(4068.f, -6337.f, 168.f));
	//SpawnTransform = FTransform(FRotator().ZeroRotator, GetOwner()->GetActorLocation());
}

void UObjectPool::BeginPlay()
{
	Super::BeginPlay();

	if (PooledObjectClass != nullptr)
	{
		for (int32 i = 0; i < PoolSize; i++)
		{
			FTransform Transform = FTransform(FRotator().ZeroRotator, GetOwner()->GetActorLocation());
			APooledObject_Sphere* Bullet = GetWorld()->SpawnActorDeferred<APooledObject_Sphere>(PooledObjectClass, Transform);
			Bullet->SetOwner(GetOwner());
			Bullet->SetActive(false);
			Bullet->SetPoolIndex(PoolIndex++);
			Bullet->OnPooledObjectDespawn.AddDynamic(this, &UObjectPool::PooledObjectDespawn);
			BulletPool.Add(Bullet);
			UGameplayStatics::FinishSpawningActor(Bullet, Transform);
		}
	}
}

//UE_LOG(LogTemp, Log, TEXT("SphereObject Start Loc : (%f,%f,%f)"), Start.Z, Start.Y, Start.Z);

APooledObject_Sphere* UObjectPool::SpawnPooledObject(FVector Start, FVector Dir)
{
	// BulletPool¿¡¼­ ²¨³»±â
	for (APooledObject_Sphere* SphereObject : BulletPool)
	{
		if (SphereObject != nullptr && !SphereObject->IsActive())
		{
			if (GetOwner()->HasAuthority())
			{
				SphereObject->SetActive(true);
			}
			SphereObject->SetActorLocation(Start);
			SphereObject->AddForceToBullet(Dir);
			SphereObject->AttackComplete = false;
			SpawnedPoolIndexes.Add(SphereObject->GetPoolIndex());
			return SphereObject;
		}
	}

	if (SpawnedPoolIndexes.Num() > 0)
	{
		int32 PooledObjectIndex = SpawnedPoolIndexes[0];
		SpawnedPoolIndexes.Remove(PooledObjectIndex);
		APooledObject_Sphere* SphereObject = BulletPool[PooledObjectIndex];
		if (SphereObject != nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("SphereObject Pool Index Num > 0 "));
			SphereObject->SetActive(false);
			SphereObject->SetActorLocation(Start);
			SphereObject->SetActive(true);
			SphereObject->AttackComplete = false;
			SphereObject->AddForceToBullet(Dir);
			//SphereObject->SetLifeSpan(PooledObjectLifeSpan);
			SpawnedPoolIndexes.Add(SphereObject->GetPoolIndex());
			return SphereObject;
		}
	}

	return nullptr;
}


void UObjectPool::PooledObjectDespawn(APooledObject* PoolActor)
{
	SpawnedPoolIndexes.Remove(PoolActor->GetPoolIndex());
}

