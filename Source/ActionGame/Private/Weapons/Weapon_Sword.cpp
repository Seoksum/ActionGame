// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon_Sword.h"
#include "Characters/ActionGameCharacter.h"
#include "GameData/GameCollision.h"
#include "Kismet/GameplayStatics.h"


AWeapon_Sword::AWeapon_Sword()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	bReplicates = true;
}

void AWeapon_Sword::BeginPlay()
{
	Super::BeginPlay();

}

void AWeapon_Sword::SwordAttack(float InDamage, float InTraceDistance, float InRadius)
{
	if (MyPawn)
	{
		FHitResult Hit;
		FCollisionShape SweepShape = FCollisionShape::MakeSphere(InRadius);
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(MyPawn);

		FVector TraceStart = GetActorLocation();
		FVector TraceEnd = TraceStart + (MyPawn->GetActorForwardVector() * InTraceDistance);

		bool bResult = GetWorld()->SweepSingleByChannel(Hit, TraceStart, TraceEnd, FQuat::Identity, ATTACK, SweepShape, Params);
		if (bResult)
		{
			FDamageEvent DamageEvent;
			Hit.Actor->TakeDamage(InDamage, DamageEvent, MyPawn->GetController(), this);
			if (Particle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, Hit.Actor->GetTransform());
			}
		}
	}
}

void AWeapon_Sword::SwordAttackUltimate(float InRadius, float InTraceDistance, float InDamage)
{
	if (MyPawn)
	{
		TArray<FHitResult> TraceHits;
		FCollisionShape SweepShape = FCollisionShape::MakeSphere(InRadius);
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActor(MyPawn);

		FVector TraceStart = GetActorLocation();
		FVector TraceEnd = TraceStart + (MyPawn->GetActorForwardVector() * InTraceDistance);

		bool bResult = GetWorld()->SweepMultiByChannel(TraceHits, TraceStart, TraceEnd, FQuat::Identity, ATTACK, SweepShape, Params);
		if (bResult)
		{
			for (FHitResult& Hit : TraceHits)
			{
				FDamageEvent DamageEvent;
				Hit.Actor->TakeDamage(InDamage, DamageEvent, MyPawn->GetController(), this);
				if (Particle)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, Hit.Actor->GetTransform());
				}
			}
		}
	}
}


//if (bResult)
//{
//	FDamageEvent DamageEvent;
//	UGameplayStatics::ApplyPointDamage(Hit.GetActor(), InDamage, GetActorForwardVector(), Hit, MyPawn->GetController(), this, DamageType);
//}