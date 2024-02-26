// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseCharacterAnimInstance.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnSpawnSphere);
DECLARE_MULTICAST_DELEGATE(FOnHealDelegate);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAttackHit, float, float, float);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnUltimateAttackHit, float, float, float);


UCLASS()
class ACTIONGAME_API UBaseCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:

	UBaseCharacterAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	void JumpToSection(int32 SectionIndex, UAnimMontage* Animation);
	FName GetAttackMontageName(int32 SectionIndex);


private:

	UFUNCTION()
	void AnimNotify_SpawnSphereNotify();

	UFUNCTION()
	void AnimNotify_HealNotify();

	UFUNCTION()
	void AnimNotify_AttackHit();

	UFUNCTION()
	void AnimNotify_AttackHit_Q();

	UFUNCTION()
	void AnimNotify_AttackHit_E();

	UFUNCTION()
	void AnimNotify_AttackHit_R();



protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = true))
	float Direction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = true))
	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = true))
	bool IsDeath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = true))
	int32 WeaponIdx;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = true))
	class UStatComponent* Stat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State", Meta = (AllowPrivateAccess = true))
	bool IsOnWall;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float TraceDistance;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float Radius;


	// Delegate
public:

	FOnSpawnSphere OnSpawnSphere;
	FOnHealDelegate OnHealDelegate;
	
	FOnAttackHit OnAttackHit;
	FOnUltimateAttackHit OnUltimateAttackHit;
	


};
