// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/PooledObject_Sphere.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Enemies/Enemy.h"



APooledObject_Sphere::APooledObject_Sphere()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp->OnComponentHit.AddDynamic(this, &APooledObject_Sphere::OnHit);

	ForceValue = 1000.f;
	bUseVelocityChange = true;

	bReplicates = true;
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

	if (GetOwner()->HasAuthority())
	{
		if (Active)
		{
			FindEnemyPath();
			NextPathPoint = GetNextPathPoint();
			FVector ForceDirection = NextPathPoint - GetActorLocation();
			ForceDirection.Normalize();
			ForceDirection *= ForceValue;
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
			return;
		}
	}
}

FVector APooledObject_Sphere::GetNextPathPoint()
{
	if (Enemy)
	{
		UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(GetOwner()->GetWorld(), GetActorLocation(), Enemy);
		if (NavPath->PathPoints.Num() > 1)
		{
			return NavPath->PathPoints[1];
		}
	}
	return GetActorLocation();
}

void APooledObject_Sphere::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (AttackComplete)
	{
		return;
	}

	AEnemy* HitEnemy = Cast<AEnemy>(OtherActor);
	if (HitEnemy)
	{
		FDamageEvent DamageEvent;
		HitEnemy->TakeDamage(15.f, DamageEvent, OtherActor->GetInstigatorController(), GetOwner());
		AttackComplete = true;
		SetActive(false);
	}
}


