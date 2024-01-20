// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyAnimInstance.h"
#include "Enemies/Enemy.h"
#include "GameFramework/PawnMovementComponent.h"

UEnemyAnimInstance::UEnemyAnimInstance()
{
	


}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	BSNode();
}

void UEnemyAnimInstance::BSNode()
{
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Speed = Pawn->GetVelocity().Size();
		auto Enemy = Cast<AEnemy>(Pawn);
		if (Enemy)
		{
			SpeedSide = Enemy->GetActorRightVector().Size();
			SpeedForward = Enemy->GetActorForwardVector().Size();
			IsFalling = Enemy->GetMovementComponent()->IsFalling();
			IsDeath = Enemy->IsDeath;
		}
	}

}


void UEnemyAnimInstance::JumpToSection(int32 SectionIndex, UAnimMontage* AnimMontage)
{
	FName Name = GetAttackMontageName(SectionIndex);
	Montage_JumpToSection(Name, AnimMontage);
}

FName UEnemyAnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
}

void UEnemyAnimInstance::AnimNotify_AttackHitCorpse()
{
	OnCorpseAttackHit.Broadcast();
}

void UEnemyAnimInstance::AnimNotify_BossAttackHit()
{
	OnBossAttackHit.Broadcast();
}

void UEnemyAnimInstance::AnimNotify_SkillHit()
{
	OnSkillAttack.Broadcast();
}