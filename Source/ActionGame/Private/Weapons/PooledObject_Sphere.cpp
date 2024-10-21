// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/PooledObject_Sphere.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/ActionGameCharacter.h"
#include "Enemies/Enemy.h"
#include "Net/UnrealNetwork.h"


APooledObject_Sphere::APooledObject_Sphere()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &APooledObject_Sphere::OnBeginOverlap);

	BaseDamage = 15.f;
	bUseVelocityChange = true;

	bReplicates = true;
	OnRep_ReplicateMovement();
	SetReplicateMovement(true);

}

void APooledObject_Sphere::BeginPlay()
{
	Super::BeginPlay();

	//bIsMeshVisiblity = false;
	AttackComplete = false;
	Active = false;
}

void APooledObject_Sphere::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}

// Called every frame
void APooledObject_Sphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (Active)
		{
			FindEnemyPath();
			NextPathPoint = GetNextPathPoint();
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();
			ForceDirection *= ForceSize;
			MeshComp->AddForce(ForceDirection, NAME_None, bUseVelocityChange);
		}
	}
}

void APooledObject_Sphere::FindEnemyPath()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), EnemyClass, Actors);

	for (AActor* Actor : Actors)
	{
		AEnemy* FindEnemy = Cast<AEnemy>(Actor);
		if (FindEnemy)
		{
			if (FindEnemy->IsDeath)
				continue;

			Enemy = FindEnemy;
			//UE_LOG(LogTemp, Log, TEXT("Enemy Name : %s  !! "), *Enemy->GetName());
			return;
		}
	}
}

FVector APooledObject_Sphere::GetNextPathPoint()
{
	if (Enemy)
	{
		UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), MyPawn->GetActorLocation(), Enemy);
		if (NavPath->PathPoints.Num() > 1)
		{
			return NavPath->PathPoints[1];
		}
	}
	return GetActorLocation();
}

void APooledObject_Sphere::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AttackComplete)
	{
		return;
	}

	AEnemy* HitEnemy = Cast<AEnemy>(OtherActor);
	if (HitEnemy)
	{
		FDamageEvent DamageEvent;
		HitEnemy->TakeDamage(BaseDamage, DamageEvent, MyPawn->GetController() , this);

		AttackComplete = true;
		SetActive(false);
		OnPooledObjectDespawn.Broadcast(this);
		MulticastPlayEffect(HitEnemy->GetTransform(), AttackParticle, AttackSound);
	}
}

void APooledObject_Sphere::MulticastPlayEffect_Implementation(FTransform SpawnTransform, UParticleSystem* Particle, USoundBase* Sound)
{
	if (Particle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, SpawnTransform);
	}
	if (Sound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Sound, SpawnTransform.GetLocation());
	}
}

