// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/CharacterWidgetInterface.h"
#include "Enemy.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyDeath, float);

UCLASS()
class ACTIONGAME_API AEnemy : public ACharacter, public ICharacterWidgetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHealthChanged(float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);


	UFUNCTION(NetMulticast, Reliable)
		void MulticastPlayAnimation(UAnimMontage* Animation);
	void MulticastPlayAnimation_Implementation(UAnimMontage* Animation);


public:

	virtual void Attack();

	UFUNCTION(Reliable, Server)
	void ServerAttack();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);



protected:

	virtual void SetupCharacterWidget(class UPlayerUserWidget* InUserWidget) override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UPlayerWidgetComponent* HpBar;

	UPROPERTY(VisibleAnywhere, Category = "AnimInstance")
	class UEnemyAnimInstance* AnimInstance;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStatComponent* Stat;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	bool IsAttacking = false;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* Weapon;

	//UPROPERTY(EditAnywhere, Category = "Mesh")
	//class USkeletalMeshComponent* WeaponLeft;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* DeathAnim;

	UPROPERTY(EditAnywhere, Category = "Stat")
	int32 Level;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float EnemyExp;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	int32 EnemyType;

	UPROPERTY(ReplicatedUsing = OnRep_AttackIndex)
	int32 AttackIndex;

	UFUNCTION()
	void OnRep_AttackIndex();

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	int32 MaxAttackIndex;

	UPROPERTY(EditAnywhere, Category = "Particle Effect")
	class USoundBase* DeathSound;


public:

	UPROPERTY(ReplicatedUsing = OnRep_Death)
	bool IsDeath;

	UFUNCTION()
	void OnRep_Death();
	
	FTimerHandle DeathTimerHandle;

	FOnAttackEnd OnAttackEnd;

	FOnEnemyDeath OnEnemyDeath;

	virtual void EnemyDeath();

	UPROPERTY(EditAnywhere, Category = "Particle")
	class UParticleSystem* HitParticle;


	// Item
protected:

	void SpawnItem();

	UPROPERTY(EditAnywhere, Category = "Enemy")
	bool PotionVal;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AItem> HpPotionToSpawn;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AItem> ManaPotionToSpawn;


};
