// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/CharacterStat.h"
#include "StatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnStatChanged, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHpChanged, float, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnManaChanged, float, float);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnHealthChanged, float, Health, float, HealthDelta, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerLevelUp, float);
DECLARE_MULTICAST_DELEGATE(FOnHpZero);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONGAME_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UStatComponent();

	void SetLevelStat(float NewLevel);
	
	UFUNCTION()
	void SetHp(float Hp);

	void SetMana(float Mana);

	void OnAttacked(float DamageAmount);
	void OnAttacking(float ManaAmount);

	void DrinkHpPotion(float HealAmount);
	void DrinkManaPotion(float ManaAmount);

	void SetExp(float Exp);
	
	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE float GetMaxHp() const { return MaxHp; }
	FORCEINLINE float GetCurrentMana() const { return CurrentMana; }
	FORCEINLINE float GetMaxMana() const { return MaxMana; }

	FORCEINLINE float GetHpRatio() const { return (CurrentHp / MaxHp); }

	void ResetStat(float NewLevel);

protected:

	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

	bool bIsDead;


	UPROPERTY(ReplicatedUsing = OnRep_UpdateHp,Transient, BlueprintReadOnly, Category = "Stat")
	float CurrentHp;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateHp,Transient, VisibleInstanceOnly, Category = "Stat")
	float MaxHp;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateMana,Transient, VisibleInstanceOnly, Category = "Stat")
	float CurrentMana;

	UPROPERTY(ReplicatedUsing = OnRep_UpdateMana, Transient,VisibleInstanceOnly, Category = "Stat")
	float MaxMana;

	UPROPERTY(ReplicatedUsing = OnRep_SetLevel, Transient, VisibleInstanceOnly, Category = "Stat")
	float CurrentLevel;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat")
	float MaxLevel;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Stat")
	FCharacterStat BaseStat;

	UPROPERTY(VisibleInstanceOnly, Category = "Stat")
	float CurrentExp;

protected:

	UFUNCTION()
	void OnRep_UpdateHp();

	UFUNCTION()
	void OnRep_UpdateMana();

	UFUNCTION()
	void OnRep_SetLevel();


public:

	FOnHpChanged OnHpChanged;
	FOnManaChanged OnManaChanged;


	FOnStatChanged OnStatChanged;
	FOnHpZero OnHpZero;
		

	FORCEINLINE FCharacterStat GetStat() const { return BaseStat; }


	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	FOnPlayerLevelUp OnPlayerLevelUp;

	FOnHealthChanged OnHealthChanged;


private:
	
	UPROPERTY()
	class UActionGameInstance* GameInstance;

};
