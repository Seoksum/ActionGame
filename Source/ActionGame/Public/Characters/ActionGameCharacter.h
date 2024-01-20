// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/PlayerStateInterface.h"
#include "Interface/CharacterItemInterface.h"
#include "Interface/CharacterWidgetInterface.h"
#include "Interface/HealthInterface.h"
#include "ActionGameCharacter.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnPickUpItem);

UCLASS(config = Game)
class AActionGameCharacter : public ACharacter, public IPlayerStateInterface, public ICharacterWidgetInterface, public ICharacterItemInterface, public IHealthInterface
{
	GENERATED_BODY()

public:
	AActionGameCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }


protected:

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	virtual void Tick(float DeltaTime) override;

	void UpDown(float Val);
	void MoveRight(float Val);

	virtual void OnDeath_Implementation() override;

	UPROPERTY(Transient, ReplicatedUsing=OnRep_Death)
	bool bIsDeath;

	UFUNCTION()
	void OnRep_Death();

	// Player Respawn 
public:

	void RespawnCharacter();

	UFUNCTION(Reliable, Server)
	void ServerRespawnCharacter();

	UPROPERTY(EditAnywhere, Category = "Respawn")
	FName TagName;

	UPROPERTY(VisibleAnywhere,Category="Respawn")
	TArray<AActor*> PlayerStarts;


public:

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;



	// Attack
protected:

	void Attack();
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerAttack();
	bool ServerAttack_Validate();

	void AttackQ();
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerAttackQ();
	bool ServerAttackQ_Validate();

	void AttackE();
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerAttackE();
	bool ServerAttackE_Validate();

	void AttackR();
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerAttackR();
	bool ServerAttackR_Validate();



	//UPROPERTY(ReplicatedUsing = OnRep_IsAttacking)
	UPROPERTY(VisibleAnywhere, Category = "Attack")
	bool IsAttacking = false;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	bool IsAttackingQ = false;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	bool IsAttackingE = false;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	bool IsAttackingR = false;

	UPROPERTY(ReplicatedUsing = OnRep_AttackIndex)
	int32 AttackIndex;

	UFUNCTION()
	void OnRep_AttackIndex();

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	int32 MaxAttackIndex;

	FTimerHandle TimerHandle_SkillCoolQ;
	FTimerHandle TimerHandle_SkillCoolE;
	FTimerHandle TimerHandle_SkillCoolR;



	// Stat
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStatComponent* Stat;

	UPROPERTY(VisibleAnywhere)
	class UPlayerWidgetComponent* StatWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SkillMana;

	virtual void SetupCharacterWidget(class UPlayerUserWidget* InUserWidget) override;

	UPROPERTY(Transient, Replicated)
	class AWeapon* CurrentWeapon;

	UPROPERTY(EditAnywhere, Category = "Attack")
	int32 CurrentWeaponIndex;

	// Level
public:

	//UPROPERTY(ReplicatedUsing = OnRep_LevelUp, Transient, VisibleInstanceOnly, Category = "Stat")
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UParticleSystem* LevelupEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USoundBase* LevelupSound;

	void IncreaseExp(float Exp);

	void LevelUp(float PlayerLevel);

	//UFUNCTION()
	//void OnRep_LevelUp();




	// AnimInstance & Animation
public:

	UPROPERTY(VisibleAnywhere)
	class UBaseCharacterAnimInstance* AnimInstance;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlayAnimation(UAnimMontage* Animation);

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* AttackQ_Anim;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* AttackE_Anim;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* AttackR_Anim;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* ClimbingUp_Anim;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* ClimbingComplete_Anim;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* Death_Anim;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
	UAnimMontage* Respawn_Anim;


	// Climbing
public:

	void PressClimbingUp(); // 1번 키를 눌러 캐릭터 벽 타기

	UFUNCTION(Reliable, Server)
	void ServerPressClimbingUp(UAnimMontage* AnimMontage, bool flag);

	void ReleaseClimbing();

	FTimerHandle ClimbingTimerHandle;

	//UPROPERTY(ReplicatedUsing = OnRep_ClimbingUp, VisibleAnywhere, Category = "Climbing")
	UPROPERTY(VisibleAnywhere, Category = "Climbing")
	bool bIsClimbingUp;

	UPROPERTY(Replicated)
	bool bIsOnWall;

	UPROPERTY(VisibleAnywhere, Category = "Climbing")
	bool bIsClimbingComplete = false;

	UPROPERTY(VisibleAnywhere, Category = "Climbing")
	bool CanPressClimbingUp = true;

	//UPROPERTY(VisibleAnywhere, Category = "Climbing")
	UPROPERTY(ReplicatedUsing = OnRep_ClimbingUp, VisibleAnywhere, Category = "Climbing")
	FVector HangingLocation;

	UFUNCTION()
	void OnRep_ClimbingUp();

	UPROPERTY(EditAnywhere, Category = "Climbing")
	float diff;


	// Inventory & Item
public:

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	class UInventoryComponent* Inventory;

	//UPROPERTY(Replicated)
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	class AItem* CurrentInteractable;

	void Interact();

	//UFUNCTION(BlueprintPure, Category = "Inventory Functions")
	void AddItemToInventory(class AItem* Item);

	//라인트레이스를 이용해서 플레이어 앞에 있는 액터가 interactable가능한 아이템인지 확인해주는 함수
	virtual void CheckForInteractable(class AItem* Item) override;

	void UseItem(class AItem* Item);

	UFUNCTION(Reliable, Server)
	void ServerUseItem(class AItem* Item);

	void SelectInventory();

	FOnPickUpItem OnPickUpItem;



	// Interface 
public:

	virtual class UStatComponent* GetMyStatComponent() override;
	virtual class UInventoryComponent* GetMyInventoryComponent() override;

	virtual bool GetIsDeath() override;
	virtual int32 GetCurrentWeaponIndex() override;
	virtual bool GetIsOnWall() override;
	virtual bool GetIsClibmingUp() override;


	// InGameWidget 
protected:


	UPROPERTY()
		class USkillWidget* SkillWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		int32 CoolDownTime_Q;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		int32 CoolDownTime_E;

	UPROPERTY(EditDefaultsOnly, Category = "Skill")
		int32 CoolDownTime_R;

	void StartSkillCoolTime(int32 Remaining_Skill, char ch);
	void SetSkillWidget();

	void EnableMouseCursor();
	void DisableMouseCursor();

};
