// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledObject.h"
#include "ObjectPool.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
APooledObject::APooledObject()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(35.f);
	CollisionComp->SetCollisionProfileName(FName("Bullet"));
	CollisionComp->SetSimulatePhysics(false);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = MeshComp;
	ProjectileMovement->ProjectileGravityScale = 1.f;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;

	ForceSize = 1000.f;
	bReplicates = true;
	LifeSpan = 3.f;
	Active = true;
}


void APooledObject::SetActive(bool IsActive)
{
	if (HasAuthority())
	{
		Active = IsActive;
		MeshComp->SetVisibility(Active);
		//GetWorldTimerManager().SetTimer(LifeSpanTimer, this, &APooledObject::Deactivate, LifeSpan, false);
		if (Active)
		{
			GetWorldTimerManager().SetTimer(LifeSpanTimer, FTimerDelegate::CreateLambda([this] {
				Active = false;
				OnRep_SetActiveBullet();
				OnPooledObjectDespawn.Broadcast(this);
				GetWorldTimerManager().ClearTimer(LifeSpanTimer);
				}), LifeSpan, false);
		}
	}
}

void APooledObject::OnRep_SetActiveBullet()
{
	MeshComp->SetVisibility(Active);
	//MeshComp->SetSimulatePhysics(Active);
}

void APooledObject::SetPoolIndex(int32 Index)
{
	PoolIndex = Index;
}

void APooledObject::Deactivate()
{
	if (HasAuthority())
	{
		Active = false;
		OnRep_SetActiveBullet();
	}

	GetWorldTimerManager().ClearTimer(LifeSpanTimer);
	OnPooledObjectDespawn.Broadcast(this);
}

bool APooledObject::IsActive()
{
	return Active;
}

int32 APooledObject::GetPoolIndex()
{
	return PoolIndex;
}

void APooledObject::AddForceToBullet(FVector Dir)
{
	MeshComp->AddForce(Dir * ForceSize, NAME_None, true);
}

void APooledObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(APooledObject, bIsMeshVisiblity);
	DOREPLIFETIME(APooledObject, Active);
	
}
