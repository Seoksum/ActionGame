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
	TraceDistance = 500.f;
	Radius = 200.f;
	HealAmount = -10.f;

	bReplicates = true;
	OnRep_ReplicateMovement();
	SetReplicateMovement(true);
	//GetMesh()->MeshComponentUpdateFlag = EMeshComponentUpdateFlag::AlwaysTickPoseAndRefreshBones;

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
}

//if (HasAuthority())
//{
//	SocketLocation = GetMesh()->GetSocketLocation(FName("RightHandSocket"));
//	ShotDirection = (GetActorForwardVector().Rotation()).Vector() * 20.f;
//}

//UE_LOG(LogTemp, Log, TEXT("RightHandSocket Start Loc : (%f,%f,%f)"), NewSocketLocation.X, NewSocketLocation.Y, NewSocketLocation.Z);

void AActionGameCharacter_Maze::SkillQ_Maze()
{
	SphereObject = ObjectPool->SpawnPooledObject();
	FVector SocketLocation = GetMesh()->GetSocketLocation(FName("RightHandSocket"));

	if (SphereObject)
	{
		SphereObject->SetActorLocation(SocketLocation);
		SphereObject->SetActive(true);
	}
}

void AActionGameCharacter_Maze::SkillE_Maze()
{
	TArray<FHitResult> TraceHits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = TraceStart + (GetActorForwardVector() * TraceDistance);
	FCollisionShape SweepShape = FCollisionShape::MakeSphere(Radius);

	bool bResult = GetWorld()->SweepMultiByChannel(TraceHits, TraceStart, TraceEnd, FQuat::Identity, ENEMY_ATTACK, SweepShape, Params);
	if (bResult)
	{
		for (FHitResult& Hit : TraceHits)
		{
			FDamageEvent DamageEvent;
			AActionGameCharacter* Player = Cast<AActionGameCharacter>(Hit.Actor);
			UGameplayStatics::ApplyPointDamage(Player, HealAmount, GetActorForwardVector(), Hit, GetController(), this, DamageType);

			if (HealParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HealParticle, Hit.ImpactPoint, FRotator(0.f, 0.f, 0.f), true);
			}
		}
	}
}

void AActionGameCharacter_Maze::SkillUltimate_Maze(float InDamage, float InTraceDistance, float InRadius)
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
}
