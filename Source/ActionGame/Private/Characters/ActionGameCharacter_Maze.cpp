// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ActionGameCharacter_Maze.h"
#include "Characters/ActionGameCharacter.h"
#include "Characters/BaseCharacterAnimInstance.h"
#include "GameData/GameEnum.h"
#include "ObjectPool.h"
#include "Weapons/PooledObject_Sphere.h"
#include "Kismet/GameplayStatics.h"
#include "GameData/GameCollision.h"
#include "Net/UnrealNetwork.h"
#include "../ActionGame.h"


AActionGameCharacter_Maze::AActionGameCharacter_Maze()
{
	ObjectPool = CreateDefaultSubobject<UObjectPool>(TEXT("ObjectPool"));


	IsAttacking = false;
	HealAmount = -10.f;

	bReplicates = true;
	OnRep_ReplicateMovement();
	SetReplicateMovement(true);
}

void AActionGameCharacter_Maze::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AActionGameCharacter_Maze::BeginPlay()
{
	Super::BeginPlay();


}

void AActionGameCharacter_Maze::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	AnimInstance = Cast<UBaseCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnAttackHit.AddUObject(this, &AActionGameCharacter_Maze::DefaultAttack);
		AnimInstance->OnSpawnSphere.AddUObject(this, &AActionGameCharacter_Maze::SkillQ_Maze);
		AnimInstance->OnHealDelegate.AddUObject(this, &AActionGameCharacter_Maze::SkillE_Maze);
		AnimInstance->OnUltimateAttackHit.AddUObject(this, &AActionGameCharacter_Maze::SkillUltimate_Maze);

	}
}

void AActionGameCharacter_Maze::DefaultAttack(float InDamage, float InTraceDistance, float InRadius)
{
	TArray<FHitResult> TraceHits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = TraceStart + (GetActorForwardVector() * InTraceDistance);
	FCollisionShape SweepShape = FCollisionShape::MakeSphere(InRadius);

	bool bResult = GetWorld()->SweepMultiByChannel(TraceHits, TraceStart, TraceEnd, FQuat::Identity, ATTACK, SweepShape, Params);
	if (bResult)
	{
		for (FHitResult& Hit : TraceHits)
		{
			FDamageEvent DamageEvent;
			Hit.Actor->TakeDamage(InDamage, DamageEvent, GetController(), this);
			if (AttackParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackParticle, Hit.Actor->GetTransform());
			}
		}
	}
	//NET_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));
}

void AActionGameCharacter_Maze::SkillQ_Maze()
{
	if (HasAuthority())
	{
		SocketLocation = GetMesh()->GetSocketLocation(FName("RightHandSocket"));
		ShotDirection = (GetActorForwardVector().Rotation()).Vector() * 20.f;
		
	}

	FVector NewSocketLocation = GetMesh()->GetSocketLocation(FName("RightHandSocket"));
	FVector NewShotDirection = (GetActorForwardVector().Rotation()).Vector() * 20.f;
	ObjectPool->SpawnPooledObject(NewSocketLocation, NewShotDirection);
}

void AActionGameCharacter_Maze::SkillE_Maze()
{
	TArray<FHitResult> TraceHits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = TraceStart + (GetActorForwardVector() * 500);
	FCollisionShape SweepShape = FCollisionShape::MakeSphere(200.0f);

	bool bResult = GetWorld()->SweepMultiByChannel(TraceHits, TraceStart, TraceEnd, FQuat::Identity, ENEMY_ATTACK, SweepShape, Params);
	if (bResult)
	{
		for (FHitResult& Hit : TraceHits)
		{
			FDamageEvent DamageEvent;
			AActionGameCharacter* Player = Cast<AActionGameCharacter>(Hit.Actor);
			UGameplayStatics::ApplyPointDamage(Player, HealAmount, GetActorForwardVector(), Hit, GetController(), this, DamageType);

			if (Particle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, Hit.ImpactPoint, FRotator(0.f, 0.f, 0.f), true);
			}
		}
	}
}

void AActionGameCharacter_Maze::SkillUltimate_Maze(float Damage, float TraceDistance, float Radius)
{
	TArray<FHitResult> TraceHits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = TraceStart + (GetActorForwardVector() * TraceDistance);
	FCollisionShape SweepShape = FCollisionShape::MakeSphere(Radius);

	bool bResult = GetWorld()->SweepMultiByChannel(TraceHits, TraceStart, TraceEnd, FQuat::Identity, ATTACK, SweepShape, Params);
	if (bResult)
	{
		for (FHitResult& Hit : TraceHits)
		{
			FDamageEvent DamageEvent;
			Hit.Actor->TakeDamage(Damage, DamageEvent, GetController(), this);
			if (AttackParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackParticle, Hit.Actor->GetTransform());
			}
		}
	}
}

void AActionGameCharacter_Maze::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(AActionGameCharacter_Maze, HitScanTrace);
	DOREPLIFETIME(AActionGameCharacter_Maze, SocketLocation);
	DOREPLIFETIME(AActionGameCharacter_Maze, ShotDirection);


}