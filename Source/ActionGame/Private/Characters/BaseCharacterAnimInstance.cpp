// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacterAnimInstance.h"
#include "Characters/ActionGameCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Interface/PlayerStateInterface.h"
#include "Components/StatComponent.h"


UBaseCharacterAnimInstance::UBaseCharacterAnimInstance()
{


}

void UBaseCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	IPlayerStateInterface* PlayerState = Cast<IPlayerStateInterface>(TryGetPawnOwner());
	if (PlayerState)
	{
		Stat = PlayerState->GetMyStatComponent();
	}
}

void UBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		if (Pawn)
		{
			Speed = Pawn->GetVelocity().Size();
			Direction = CalculateDirection(Pawn->GetVelocity(), Pawn->GetActorRotation());
			IsFalling = Pawn->GetMovementComponent()->IsFalling();
		}

		IPlayerStateInterface* PlayerState = Cast<IPlayerStateInterface>(TryGetPawnOwner());
		if (PlayerState)
		{
			IsDeath = PlayerState->GetIsDeath();
			IsOnWall = PlayerState->GetIsOnWall();
		}
	}
}


void UBaseCharacterAnimInstance::JumpToSection(int32 SectionIndex, UAnimMontage* Animation)
{
	FName Name = GetAttackMontageName(SectionIndex);
	Montage_JumpToSection(Name, Animation);
}

FName UBaseCharacterAnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
}

void UBaseCharacterAnimInstance::AnimNotify_SpawnSphereNotify()
{
	OnSpawnSphere.Broadcast();
}

void UBaseCharacterAnimInstance::AnimNotify_HealNotify()
{
	OnHealDelegate.Broadcast();
}

void UBaseCharacterAnimInstance::AnimNotify_AttackHit()
{
	TraceDistance = Stat->GetStat().AttackRange;
	Radius = 200.f;
	OnAttackHit.Broadcast(Stat->GetStat().Attack, TraceDistance, Radius);
}

void UBaseCharacterAnimInstance::AnimNotify_AttackHit_Q()
{
	TraceDistance = Stat->GetStat().AttackRange * 2.f;
	Radius = 200.f;
	OnAttackHit.Broadcast(Stat->GetStat().AttackQ, TraceDistance, Radius);
}

void UBaseCharacterAnimInstance::AnimNotify_AttackHit_E()
{
	TraceDistance = Stat->GetStat().AttackRange * 2.5f;
	Radius = 400.f;
	OnAttackHit.Broadcast(Stat->GetStat().AttackE, TraceDistance, Radius);
}

void UBaseCharacterAnimInstance::AnimNotify_AttackHit_R()
{
	TraceDistance = Stat->GetStat().AttackRange * 3.f;
	Radius = 500.f;
	OnUltimateAttackHit.Broadcast(Stat->GetStat().AttackR, TraceDistance, Radius);
}

