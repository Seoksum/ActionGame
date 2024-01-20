// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy_Boss.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StatComponent.h"
#include "Enemies/EnemyAnimInstance.h"
#include "AI/AIController_Enemy.h"
//#include "GameData/GameEnum.h"
#include "GameData/GameCollision.h"
#include "GameData/GameEnum.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"


AEnemy_Boss::AEnemy_Boss()
{
	//Stat = CreateDefaultSubobject<UMyStatComponent>(TEXT("STAT"));

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightWeaponMesh"));
	WeaponLeft = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftWeaponMesh"));
	Weapon->SetupAttachment(GetMesh());
	WeaponLeft->SetupAttachment(GetMesh());

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SwordMeshLeft(TEXT("SkeletalMesh'/Game/CityofBrass_Enemies/Meshes/Enemy/Sword/enemy_Sword_Mesh.enemy_Sword_Mesh'"));
	if (SwordMeshLeft.Succeeded())
	{
		Weapon->SetSkeletalMesh(SwordMeshLeft.Object);
		WeaponLeft->SetSkeletalMesh(SwordMeshLeft.Object);
	}

	AttackIndex = -1;
	Level = 5;
	EnemyExp = 100;
	EnemyType = EEnemyType::BossEnemy;
}

void AEnemy_Boss::BeginPlay()
{
	Super::BeginPlay();

	Stat->SetLevelStat(Level);


}

void AEnemy_Boss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("SwordSocket"));
	}
	if (WeaponLeft)
	{
		WeaponLeft->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LeftSwordSocket"));
	}

	if (AnimInstance)
	{
		AnimInstance->OnBossAttackHit.AddUObject(this, &AEnemy_Boss::AttackCheck);
		AnimInstance->OnSkillAttack.AddUObject(this, &AEnemy_Boss::SkillAttack);
	}


}

void AEnemy_Boss::Attack()
{
	Super::Attack();

	//if (IsAttacking || Stat->GetCurrentHp() <= 0.f)
	//	return;

	//if (HasAuthority())
	//{
	//	AttackIndex = (AttackIndex + 1) % 4;
	//	OnRep_AttackIndex();
	//}
	//else
	//{
	//	ServerNextAttack();
	//}

	// 체력이 반 아래로 떨어지면 점프
	if (Stat->GetHpRatio() < 0.5f && !JumpAttacking)
	{
		Jump();
		FVector Location = GetActorLocation();
		Location.Z += 200.f;
		SetActorLocation(Location);
		JumpAttacking = true;
	}
}

//void AEnemy_Boss::ServerNextAttack_Implementation()
//{
//	AttackIndex = (AttackIndex + 1) % 4;
//	AnimInstance->JumpToSection(AttackIndex, AttackAnim);
//	UE_LOG(LogTemp, Log, TEXT("[MYLOG] : Attack Index : %d"), AttackIndex);
//}
//
//void AEnemy_Boss::OnRep_AttackIndex()
//{
//	AnimInstance->JumpToSection(AttackIndex, AttackAnim);
//	UE_LOG(LogTemp, Log, TEXT("OnRep [MYLOG] : Attack Index : %d"), AttackIndex);
//}

void AEnemy_Boss::AttackCheck()
{
	FDamageEvent DamageEvent;
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 700.f;
	float AttackRadius = 300.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ENEMY_ATTACK,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	if (bResult)
	{
		HitResult.Actor->TakeDamage(Stat->GetStat().Attack, DamageEvent, GetController(), this);
		if (HitParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitResult.ImpactPoint, FRotator(0.f, 0.f, 0.f), true);
		}
	}
}

void AEnemy_Boss::SkillAttack()
{
	FDamageEvent DamageEvent;
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 1500.f,
		FQuat::Identity,
		ENEMY_ATTACK,
		FCollisionShape::MakeSphere(300.f),
		Params);

	if (bResult)
	{
		HitResult.Actor->TakeDamage(Stat->GetStat().Attack, DamageEvent, GetController(), this);
		if (SkillEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SkillEffect, HitResult.Actor->GetTransform());
		}
	}
}

void AEnemy_Boss::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	//FTransform Transform(FRotator::ZeroRotator, Hit.ImpactPoint, FVector(10.f, 10.f, 10.f));
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), JumpEffect, Transform);

	if (HasAuthority())
	{
		HitTransform = FTransform(FRotator::ZeroRotator, Hit.ImpactPoint, FVector(10.f, 10.f, 10.f));
		OnRep_HitTransform();
		PlayEffects(HitTransform);
	}


	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld()))
	{
		if (PlayerController)
		{
			PlayerController->ClientPlayCameraShake(AttackedCamShake);
		}
	}
}

void AEnemy_Boss::EnemyDeath()
{
	Super::EnemyDeath();

	WeaponLeft->SetVisibility(false);
}

void AEnemy_Boss::OnRep_HitTransform()
{
	PlayEffects(HitTransform);
}

void AEnemy_Boss::PlayEffects(FTransform HitLocation)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), JumpEffect, HitTransform);
}

//bool AEnemy_Boss::IsBossEnemyDead()
//{
//	return IsPawnControlled();
//}

void AEnemy_Boss::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemy_Boss, HitTransform);

}