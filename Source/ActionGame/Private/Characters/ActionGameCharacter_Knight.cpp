// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ActionGameCharacter_Knight.h"
#include "Characters/BaseCharacterAnimInstance.h"
#include "Weapons/Weapon.h"
#include "Weapons/Weapon_Sword.h"
#include "GameData/GameEnum.h"
#include "UI/InGameWidget.h"


AActionGameCharacter_Knight::AActionGameCharacter_Knight()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentWeaponIndex = EWeapon::Sword;
	AttackIndex = 0;

	bReplicates = true;
	OnRep_ReplicateMovement();
	SetReplicateMovement(true);
}

void AActionGameCharacter_Knight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AActionGameCharacter_Knight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AActionGameCharacter_Knight::BeginPlay()
{
	Super::BeginPlay();


}

void AActionGameCharacter_Knight::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		SwordWeapon = GetWorld()->SpawnActor<AWeapon_Sword>(SwordClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		if (SwordWeapon)
		{
			SwordWeapon->SetOwningPawn(this);
			SwordWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
		}
	}

	if (AnimInstance)
	{
		AnimInstance->OnAttackHit.AddUObject(this, &AActionGameCharacter_Knight::SkillAttack);
		AnimInstance->OnUltimateAttackHit.AddUObject(this, &AActionGameCharacter_Knight::SkillUltimateAttack);
	}
}

void AActionGameCharacter_Knight::SkillAttack(float Damage, float TraceDistance, float Radius)
{
	if (SwordWeapon)
	{
		SwordWeapon->SwordAttack(Damage, TraceDistance, Radius);
	}
}

void AActionGameCharacter_Knight::SkillUltimateAttack(float Damage, float TraceDistance, float Radius)
{
	if (SwordWeapon)
	{
		SwordWeapon->SwordAttackUltimate(Damage, TraceDistance, Radius);
	}
}

