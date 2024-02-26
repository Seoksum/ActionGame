// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy_ACorpse.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StatComponent.h"
#include "Enemies/EnemyAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "UI/EnemyStatWidget.h"
#include "AI/AIController_Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameData/GameEnum.h"
#include "GameData/GameCollision.h"
#include "Characters/ActionGameCharacter.h"


AEnemy_ACorpse::AEnemy_ACorpse()
{
	// Skeletal Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/CityofBrass_Enemies/Meshes/Enemy/Corpse/Corpse_Sword.Corpse_Sword'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}
	GetMesh()->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));

	// Weapon Mesh
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	Weapon->SetupAttachment(GetMesh());
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SwordMesh(TEXT("SkeletalMesh'/Game/CityofBrass_Enemies/Meshes/Enemy/Sword/enemy_Sword_Mesh.enemy_Sword_Mesh'"));
	if (SwordMesh.Succeeded())
	{
		Weapon->SetSkeletalMesh(SwordMesh.Object);
	}
	Weapon->SetCollisionProfileName(FName("NoCollision"));

	Level = 1;
	EnemyType = EEnemyType::SkeletonEnemy;
}

void AEnemy_ACorpse::BeginPlay()
{
	Super::BeginPlay();

	Stat->SetLevelStat(Level);
}

void AEnemy_ACorpse::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("SwordSocket"));
	}

	if (AnimInstance)
	{
		AnimInstance->OnCorpseAttackHit.AddUObject(this, &AEnemy_ACorpse::AttackCheck);
	}

}

void AEnemy_ACorpse::Attack()
{
	Super::Attack();

}

void AEnemy_ACorpse::AttackCheck()
{
	FHitResult Hit;
	FCollisionQueryParams Params(NAME_None, false, this);

	Params.AddIgnoredActor(this);

	float AttackRange = 500.f;
	float AttackRadius = 300.f;

	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = TraceStart + (GetActorForwardVector() * AttackRange); // 종료 지점

	bool bResult = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ENEMY_ATTACK, Params);

	if (bResult)
	{
		AActionGameCharacter* Player = Cast<AActionGameCharacter>(Hit.GetActor());
		FDamageEvent DamageEvent;
		UGameplayStatics::ApplyPointDamage(Player, Stat->GetStat().Attack, GetActorForwardVector(), Hit, GetController(), this, DamageType);
	}
}


