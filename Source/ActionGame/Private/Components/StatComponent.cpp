// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameData/MyGameSingleton.h"
#include "GameFrameworks/ActionGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/HealthInterface.h"


// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	bWantsInitializeComponent = true;

	//CurrentHp = 100;
	//MaxHp = 100;
	bIsDead = false;

	SetIsReplicatedByDefault(true);

}

// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UStatComponent::TakeDamage);
	}

	MaxLevel = 4;//static_cast<float>(GameInstance->MaxLevel);
	CurrentLevel = 1;
}

void UStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

}

void UStatComponent::SetLevelStat(float NewLevel)
{
	CurrentLevel = FMath::Clamp<float>(NewLevel, 1, MaxLevel);
	GameInstance = Cast<UActionGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	BaseStat = GameInstance->GetCharacterStat(CurrentLevel);
	MaxHp = BaseStat.MaxHp;
	MaxMana = BaseStat.MaxMana;
	SetHp(MaxHp);
	SetMana(MaxHp);
}

void UStatComponent::ResetStat(float NewLevel)
{
	SetLevelStat(NewLevel);
	bIsDead = false;
}

void UStatComponent::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	if (bIsDead)
	{
		return;
	}

	CurrentHp = FMath::Clamp(CurrentHp - Damage, 0.0f, MaxHp);
	SetHp(CurrentHp);

	bIsDead = CurrentHp <= 0.0f;
	OnHealthChanged.Broadcast(CurrentHp, Damage, DamageType, InstigatedBy, DamageCauser);
}

void UStatComponent::DrinkHpPotion(float HealAmount)
{
	const float PrevHp = CurrentHp;
	SetHp(FMath::Clamp<float>(PrevHp + HealAmount, 0.f, MaxHp));
}

void UStatComponent::DrinkManaPotion(float ManaAmount)
{
	const float PrevMana = CurrentMana;
	SetMana(FMath::Clamp<float>(PrevMana + ManaAmount, 0.f, MaxMana));
}

void UStatComponent::SetHp(float Hp)
{
	CurrentHp = FMath::Clamp<float>(Hp, 0.f, MaxHp);
	if (CurrentHp <= 0)
	{
		CurrentHp = 0;
		if (GetOwner()->Implements<UHealthInterface>())
		{
			IHealthInterface::Execute_OnDeath(GetOwner());
		}
	}
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
}

//UE_LOG(LogTemp, Log, TEXT("Current Hp : %s"), *FString::SanitizeFloat(CurrentHp));

void UStatComponent::OnRep_UpdateHp()
{
	OnHpChanged.Broadcast(CurrentHp, MaxHp);
}

void UStatComponent::OnRep_UpdateMana()
{
	OnManaChanged.Broadcast(CurrentMana, MaxMana);
}

void UStatComponent::OnAttacked(float DamageAmount)
{
	const float PrevHp = CurrentHp;
	SetHp(FMath::Clamp<float>(PrevHp - DamageAmount, 0.f, MaxHp));
}

void UStatComponent::SetMana(float Mana)
{
	CurrentMana = FMath::Clamp(Mana, 0.f, MaxMana);
	OnManaChanged.Broadcast(CurrentMana, MaxMana);
}

void UStatComponent::OnAttacking(float ManaAmount)
{
	const float PrevMana = CurrentMana;
	SetMana(FMath::Clamp(PrevMana - ManaAmount, 0.f, MaxMana));
}

void UStatComponent::SetExp(float Exp)
{
	CurrentExp += Exp;
	float LevelExp = BaseStat.Exp;
	if (LevelExp <= CurrentExp)
	{
		CurrentLevel = FMath::Clamp<float>(CurrentLevel + 1, 1, MaxLevel);
		SetLevelStat(CurrentLevel);
		OnPlayerLevelUp.Broadcast(CurrentLevel);
		CurrentExp -= LevelExp;
	}
}

void UStatComponent::OnRep_SetLevel()
{
	OnPlayerLevelUp.Broadcast(CurrentLevel);
}


void UStatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStatComponent, CurrentHp);
	DOREPLIFETIME(UStatComponent, MaxHp);
	DOREPLIFETIME(UStatComponent, CurrentMana);
	DOREPLIFETIME(UStatComponent, MaxMana);

	DOREPLIFETIME(UStatComponent, CurrentLevel);

	DOREPLIFETIME_CONDITION(UStatComponent, BaseStat, COND_OwnerOnly);



}



//GameInstance = Cast<UActionGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
//CurrentLevel = FMath::Clamp<float>(NewLevel, 1, MaxLevel);
//BaseStat = GameInstance->GetCharacterStat(CurrentLevel);
//UE_LOG(LogTemp, Log, TEXT("Current LEvel is : %s"), *FString::SanitizeFloat(CurrentLevel));
//
//bool IsFirstTime = GameInstance->GetResetLevel();
//if (IsLevelReset)
//{
//	SetHp(GetStat().MaxHp);
//	SetMana(GetStat().MaxMana);
//	GameInstance->SetResetLevel(false);
//	GameInstance->SetCurrentHp(GetStat().MaxHp);
//	GameInstance->SetCurrentMana(GetStat().MaxMana);
//}
//
//else
//{
//	SetHp(GameInstance->GetCurrentHp());
//	SetMana(GameInstance->GetCurrentMana());
//}