// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Enemy.h"
#include "Enemy_Boss.generated.h"

/**
 * 
 */


UCLASS()
class ACTIONGAME_API AEnemy_Boss : public AEnemy
{
	GENERATED_BODY()

public:

	AEnemy_Boss();


	//virtual bool IsBossEnemyDead() override;

protected:

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	virtual void Attack() override;

	//UFUNCTION(Reliable, Server)
	//void ServerNextAttack();

	void AttackCheck();

	void SkillAttack();

	virtual void Landed(const FHitResult& Hit) override;

	virtual void EnemyDeath() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particle")
	class UParticleSystem* SkillEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particle")
	class UParticleSystem* JumpEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool JumpAttacking;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<UCameraShakeBase> AttackedCamShake;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* WeaponLeft;


	UPROPERTY(ReplicatedUsing = OnRep_HitTransform)
	FTransform HitTransform;

	UFUNCTION()
	void OnRep_HitTransform();

	void PlayEffects(FTransform HitLocation);

	
};
