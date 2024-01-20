// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy.h"
#include "AI/AIController_Enemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StatComponent.h"
#include "Components/WidgetComponent.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "UI/EnemyStatWidget.h"
#include "Enemies/EnemyAnimInstance.h"
#include "UI/PlayerWidgetComponent.h"
#include "Net/UnrealNetwork.h"
#include "Interface/GameModeInterface.h"
#include "Items/Item.h"
#include "GameData/GameEnum.h"


// Sets default values
AEnemy::AEnemy()
{
	// AI Controller »ý¼º
	AIControllerClass = AAIController_Enemy::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	GetCapsuleComponent()->BodyInstance.SetCollisionProfileName(FName("Enemy"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	Stat = CreateDefaultSubobject<UStatComponent>(TEXT("STAT"));

	HpBar = CreateDefaultSubobject<UPlayerWidgetComponent>(TEXT("HPBAR"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 220.f));
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarUserWidget(TEXT("WidgetBlueprint'/Game/UI/InGame/WBP_EnemyStatWidget.WBP_EnemyStatWidget_C'"));
	if (HpBarUserWidget.Succeeded())
	{
		HpBar->SetWidgetClass(HpBarUserWidget.Class);
		HpBar->SetDrawSize(FVector2D(200.f, 50.f));
	}

	IsAttacking = false;
	bReplicates = true;
	IsDeath = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	//UEnemyStatWidget* StatBar = Cast<UEnemyStatWidget>(HpBar->GetUserWidgetObject());
	//if (StatBar)
	//{
	//	StatBar->BindHp(Stat);
	//}

}

void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	Stat->OnHealthChanged.AddDynamic(this, &AEnemy::OnHealthChanged);


	AnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AEnemy::OnAttackMontageEnded);
	}

}

void AEnemy::Attack()
{
	if (IsAttacking || IsDeath)
		return;

	IsAttacking = true;

	if (HasAuthority())
	{
		MulticastPlayAnimation(AttackAnim);
		AttackIndex = (AttackIndex + 1) % MaxAttackIndex;
		OnRep_AttackIndex();
	}
	else
	{
		ServerAttack();
	}

	//MulticastPlayAnimation(AttackAnim);
}


void AEnemy::ServerAttack_Implementation()
{
	//Attack();
	MulticastPlayAnimation(AttackAnim);
	AttackIndex = (AttackIndex + 1) % MaxAttackIndex;
	AnimInstance->JumpToSection(AttackIndex, AttackAnim);
}

void AEnemy::OnRep_AttackIndex()
{
	AnimInstance->JumpToSection(AttackIndex, AttackAnim);
}

void AEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}


void AEnemy::OnHealthChanged(float Health, float HealthDelta, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.1f && !IsDeath)
	{
		if (HasAuthority())
		{
			IsDeath = true;
			OnRep_Death();
			MulticastPlayAnimation(DeathAnim);
		}
		IGameModeInterface* GameMode = GetWorld()->GetAuthGameMode<IGameModeInterface>();
		if (GameMode)
		{
			GameMode->IncreaseExp(InstigatedBy, EnemyExp);
			GameMode->OnEnemyKilled();
		}
	}
}

void AEnemy::OnRep_Death()
{
	DetachFromControllerPendingDestroy();
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AEnemy::EnemyDeath, 1.f, false);
}

void AEnemy::EnemyDeath()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetCapsuleComponent()->BodyInstance.SetCollisionProfileName(FName("NoCollision"));
	GetMesh()->SetVisibility(false);
	Weapon->SetVisibility(false);
	HpBar->SetVisibility(false);

	GetWorldTimerManager().ClearTimer(DeathTimerHandle);

	SpawnItem();
}

void AEnemy::SpawnItem()
{
	FVector SpawnLocation = GetActorLocation();

	if (HasAuthority())
	{
		if (PotionVal)
			GetWorld()->SpawnActor<AItem>(HpPotionToSpawn, SpawnLocation, FRotator::ZeroRotator);
		else
			GetWorld()->SpawnActor<AItem>(ManaPotionToSpawn, SpawnLocation, FRotator::ZeroRotator);
	}

}

void AEnemy::MulticastPlayAnimation_Implementation(UAnimMontage* Animation)
{
	PlayAnimMontage(Animation);
}

void AEnemy::SetupCharacterWidget(UPlayerUserWidget* InUserWidget)
{
	UEnemyStatWidget* HpBarWidget = Cast<UEnemyStatWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->UpdateHp(Stat->GetCurrentHp(), Stat->GetMaxHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UEnemyStatWidget::UpdateHp);
	}

}

void AEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemy, IsDeath);
	DOREPLIFETIME(AEnemy, AttackIndex);

}