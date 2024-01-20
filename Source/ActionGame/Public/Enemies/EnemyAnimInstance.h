// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnCorpseAttackHit);
DECLARE_MULTICAST_DELEGATE(FOnBossAttackHit);
DECLARE_MULTICAST_DELEGATE(FOnSkillAttack);


/**
 * 
 */
UCLASS()
class ACTIONGAME_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:

	UEnemyAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	void BSNode();
	void PlayAttackMontage_Corpse();
	void PlayAttackMontage_Boss();

	

	void JumpToSection(int32 SectionIndex,UAnimMontage* AnimMontage);
	FName GetAttackMontageName(int32 SectionIndex);

private:

	UFUNCTION()
	void AnimNotify_AttackHitCorpse();

	UFUNCTION()
	void AnimNotify_BossAttackHit();

	UFUNCTION()
	void AnimNotify_SkillHit();



private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float SpeedSide;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	float SpeedForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool IsDeath;





public:

	FOnCorpseAttackHit OnCorpseAttackHit;

	FOnBossAttackHit OnBossAttackHit;

	FOnSkillAttack OnSkillAttack;


};
