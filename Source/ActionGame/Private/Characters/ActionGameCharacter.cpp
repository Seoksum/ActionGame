// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/ActionGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/BaseCharacterAnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StatComponent.h"
#include "Components/InventoryComponent.h"
#include "Items/Item.h"
#include "Components/WidgetComponent.h"
#include "Characters/ActionPlayerController.h"
#include "Enemies/Enemy.h"
#include "../ActionGame.h"
#include "UI/StatWidget.h"
#include "UI/SkillWidget.h"
#include "UI/PlayerWidgetComponent.h"
#include "UI/InGameWidget.h"
#include "GameData/GameEnum.h"
#include "Interface/GameModeInterface.h"



AActionGameCharacter::AActionGameCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;


	GetCapsuleComponent()->BodyInstance.SetCollisionProfileName(FName("Player"));

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	Stat = CreateDefaultSubobject<UStatComponent>(TEXT("StatComponent"));

	StatWidgetComponent = CreateDefaultSubobject<UPlayerWidgetComponent>(TEXT("HPBAR"));
	StatWidgetComponent->SetupAttachment(GetMesh());
	StatWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 220.f));
	StatWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> StatBarRef(TEXT("WidgetBlueprint'/Game/UI/InGame/WBP_StatWidget.WBP_StatWidget_C'"));
	if (StatBarRef.Succeeded())
	{
		StatWidgetComponent->SetWidgetClass(StatBarRef.Class);
		StatWidgetComponent->SetDrawSize(FVector2D(200.f, 50.f));
	}

	Level = 1.f;
	AttackIndex = -1;
	MaxAttackIndex = 3;
	diff = 150.f;

	CoolDownTime_Q = 3;
	CoolDownTime_E = 5;
	CoolDownTime_R = 7;
	CanPressClimbingUp = true;

	TagName = "PlayerStart";

	bReplicates = true;
	OnRep_ReplicateMovement();
	SetReplicateMovement(true);
}

void AActionGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &AActionGameCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AActionGameCharacter::UpDown);


	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AActionGameCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("AttackQ"), EInputEvent::IE_Pressed, this, &AActionGameCharacter::AttackQ);
	PlayerInputComponent->BindAction(TEXT("AttackE"), EInputEvent::IE_Pressed, this, &AActionGameCharacter::AttackE);
	PlayerInputComponent->BindAction(TEXT("AttackR"), EInputEvent::IE_Pressed, this, &AActionGameCharacter::AttackR);

	PlayerInputComponent->BindAction(TEXT("ClimbingUp"), EInputEvent::IE_Pressed, this, &AActionGameCharacter::PressClimbingUp);

	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AActionGameCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("Inventory"), EInputEvent::IE_Pressed, this, &AActionGameCharacter::SelectInventory);

	PlayerInputComponent->BindAction(TEXT("EnableMouseCursor"), EInputEvent::IE_Pressed, this, &AActionGameCharacter::EnableMouseCursor);
	PlayerInputComponent->BindAction(TEXT("DisableMouseCursor"), EInputEvent::IE_Pressed, this, &AActionGameCharacter::DisableMouseCursor);


}

void AActionGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 벽 위를 더 이상 오를 수 없어 착지해야할 때
	if (bIsClimbingComplete)
	{
		SetActorLocation(FVector(GetActorLocation() + FVector(-3.0f, 0.0f, 10.0f)));
	}


	// 등반 중일 때
	if (bIsOnWall)
	{
		//OnRep_ClimbingUp();
		if (bIsClimbingUp)
		{
			FVector HangingLoc = FVector(GetActorLocation() + FVector(0.0f, 0.0f, 450.0f));
			FRotator HangingRot = GetActorRotation();
			FLatentActionInfo Info;
			Info.CallbackTarget = this;
			UKismetSystemLibrary::MoveComponentTo(
				GetCapsuleComponent(),
				HangingLoc,
				HangingRot,
				true,
				true,
				0.2f,
				false,
				EMoveComponentAction::Type::Move,
				Info);
			HangingLocation = GetActorLocation();
		}
		else
		{
			SetActorLocation(HangingLocation);
		}
	}
}

void AActionGameCharacter::OnRep_ClimbingUp()
{
	//UE_LOG(LogTemp, Log, TEXT("OnRep_ClimbingUp() Func has been called  "));

	if (bIsClimbingUp)
	{
		FVector NewLocation = GetActorLocation();
		NewLocation.Z = HangingLocation.Z;
		SetActorLocation(NewLocation);
	}
	else
	{
		SetActorLocation(HangingLocation);
	}
}

void AActionGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);

	Stat->SetLevelStat(Level);

	AActionPlayerController* PlayerController = Cast<AActionPlayerController>(GetController());
	if (PlayerController/* && IsLocallyControlled()*/)
	{
		PlayerController->SetInGameLevel(Level); // 화면창 Level 업데이트 
	}

	SkillMana = Stat->GetStat().Mana;
	Stat->OnPlayerLevelUp.AddUObject(this, &AActionGameCharacter::LevelUp);

}


void AActionGameCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UBaseCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AActionGameCharacter::OnAttackMontageEnded);
	}

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TagName, PlayerStarts);
}

void AActionGameCharacter::UpDown(float Value)
{
	if (bIsOnWall || IsAttacking)
		return;

	AddMovementInput(FVector(-1.f, 0.f, 0.f), Value);
}

void AActionGameCharacter::MoveRight(float Value)
{
	if (bIsOnWall || IsAttacking)
		return;

	AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
}
//NET_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

void AActionGameCharacter::OnDeath_Implementation()
{
	if (HasAuthority())
	{
		bIsDeath = true;
		OnRep_Death();
	}
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	SetActorEnableCollision(true);
}

void AActionGameCharacter::OnRep_Death()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (bIsDeath)
	{
		MulticastPlayAnimation(Death_Anim);
		DisableInput(PlayerController);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

		AActionPlayerController* ActionPlayerController = Cast<AActionPlayerController>(GetController());
		if (ActionPlayerController)
		{
			ActionPlayerController->GameHasEnded(this, false);
		}
	}
	else
	{
		EnableInput(PlayerController);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
}

void AActionGameCharacter::RespawnCharacter()
{
	if (HasAuthority())
	{
		bIsDeath = false;
		OnRep_Death();

		int32 RandomIndex = FMath::RandRange(0, PlayerStarts.Num() - 1);
		FVector PlayerRespawnLocation = PlayerStarts[RandomIndex]->GetActorLocation();
		SetActorLocation(PlayerRespawnLocation);

		if (Stat)
		{
			Stat->ResetStat(Level);
		}

	}
	else
	{
		ServerRespawnCharacter();
	}


	//APlayerController* PlayerController = Cast<APlayerController>(GetController());
	//EnableInput(PlayerController);
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	//AnimInstance->StopAllMontages(0.0f);

}

void AActionGameCharacter::ServerRespawnCharacter_Implementation()
{
	bIsDeath = false;

	int32 RandomIndex = FMath::RandRange(0, PlayerStarts.Num() - 1);
	FVector PlayerRespawnLocation = PlayerStarts[RandomIndex]->GetActorLocation();
	SetActorLocation(PlayerRespawnLocation);

	if (Stat)
	{
		Stat->ResetStat(Level);
	}
}






void AActionGameCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void AActionGameCharacter::Attack()
{
	if (IsAttacking)
		return;

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
	IsAttacking = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void AActionGameCharacter::ServerAttack_Implementation()
{
	MulticastPlayAnimation(AttackAnim);
	AttackIndex = (AttackIndex + 1) % MaxAttackIndex;
	AnimInstance->JumpToSection(AttackIndex, AttackAnim);
}

bool AActionGameCharacter::ServerAttack_Validate()
{
	return true;
}

void AActionGameCharacter::OnRep_AttackIndex()
{
	AnimInstance->JumpToSection(AttackIndex, AttackAnim);
}

void AActionGameCharacter::AttackQ()
{
	if (IsAttackingQ || Stat->GetCurrentMana() < 0.f)
		return;

	if (!HasAuthority())
	{
		ServerAttackQ();
	}

	if (GetController()->IsLocalController())
	{
		StartSkillCoolTime(CoolDownTime_Q, 'Q');
	}

	MulticastPlayAnimation(AttackQ_Anim);
	IsAttackingQ = true;
	Stat->OnAttacking(SkillMana);

	GetWorldTimerManager().SetTimer(TimerHandle_SkillCoolQ, FTimerDelegate::CreateLambda([this] {
		IsAttackingQ = false;
		GetWorldTimerManager().ClearTimer(TimerHandle_SkillCoolQ);
		}), CoolDownTime_Q, true);
}

void AActionGameCharacter::ServerAttackQ_Implementation()
{
	AttackQ();
	MulticastPlayAnimation(AttackQ_Anim);
}

bool AActionGameCharacter::ServerAttackQ_Validate()
{
	return AttackQ_Anim != nullptr;
}

void AActionGameCharacter::AttackE()
{
	if (IsAttackingE || Stat->GetMaxMana() < 0.f)
		return;

	if (!HasAuthority())
	{
		ServerAttackE();
	}

	if (GetController()->IsLocalController())
	{
		StartSkillCoolTime(CoolDownTime_E, 'E');
	}

	MulticastPlayAnimation(AttackE_Anim);
	IsAttackingE = true;
	Stat->OnAttacking(SkillMana * 2.f);

	GetWorldTimerManager().SetTimer(TimerHandle_SkillCoolE, FTimerDelegate::CreateLambda([this] {
		IsAttackingE = false;
		GetWorldTimerManager().ClearTimer(TimerHandle_SkillCoolE);
		}), CoolDownTime_E, true);
}

void AActionGameCharacter::ServerAttackE_Implementation()
{
	AttackE();
	MulticastPlayAnimation(AttackE_Anim);
}

bool AActionGameCharacter::ServerAttackE_Validate()
{
	return AttackE_Anim != nullptr;
}

void AActionGameCharacter::AttackR()
{
	if (IsAttackingR || Stat->GetMaxMana() < 0.f)
		return;

	if (!HasAuthority())
	{
		ServerAttackR();
	}

	if (GetController()->IsLocalController())
	{
		StartSkillCoolTime(CoolDownTime_R, 'R');
	}

	IsAttackingR = true;

	Stat->OnAttacking(SkillMana * 2.5f);
	MulticastPlayAnimation(AttackR_Anim);

	GetWorldTimerManager().SetTimer(TimerHandle_SkillCoolR, FTimerDelegate::CreateLambda([this] {
		IsAttackingR = false;
		GetWorldTimerManager().ClearTimer(TimerHandle_SkillCoolR);
		}), CoolDownTime_R, true);

}

void AActionGameCharacter::StartSkillCoolTime(int32 Remaining_Skill, char ch)
{
	if (SkillWidget == nullptr)
	{
		SetSkillWidget();
	}

	SkillWidget->SetSkillCoolTime(Remaining_Skill, ch);
}

void AActionGameCharacter::ServerAttackR_Implementation()
{
	MulticastPlayAnimation(AttackR_Anim);
	AttackR();
}

bool AActionGameCharacter::ServerAttackR_Validate()
{
	return AttackR_Anim != nullptr;
}

void AActionGameCharacter::MulticastPlayAnimation_Implementation(UAnimMontage* Animation)
{
	PlayAnimMontage(Animation);
}

void AActionGameCharacter::SetupCharacterWidget(UPlayerUserWidget* InUserWidget)
{
	UStatWidget* HpBarWidget = Cast<UStatWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->UpdateHp(Stat->GetCurrentHp(), Stat->GetMaxHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UStatWidget::UpdateHp);
		Stat->OnManaChanged.AddUObject(HpBarWidget, &UStatWidget::UpdateMana);
	}
}

//NET_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));
void AActionGameCharacter::IncreaseExp(float Exp)
{
	if (HasAuthority())
	{
		Stat->SetExp(Exp);
	}
}

void AActionGameCharacter::LevelUp(float PlayerLevel)
{
	Level = PlayerLevel;
	UGameplayStatics::SpawnEmitterAttached(LevelupEffect, GetMesh());
	UGameplayStatics::PlaySoundAtLocation(this, LevelupSound, GetActorLocation());
	
	AActionPlayerController* PlayerController = Cast<AActionPlayerController>(GetController());
	if (PlayerController && IsLocallyControlled())
	{
		PlayerController->SetInGameLevel(Level); // 화면창 Level 업데이트 
	}
}

//void AActionGameCharacter::OnRep_LevelUp()
//{
//	// 파티클,사운드 생성
//	UGameplayStatics::SpawnEmitterAttached(LevelupEffect, GetMesh());
//	UGameplayStatics::PlaySoundAtLocation(this, LevelupSound, GetActorLocation());
//}

void AActionGameCharacter::PressClimbingUp()
{
	NET_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	if (!CanPressClimbingUp)
	{
		return;
	}

	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * diff;

	float CapsuleHeight = (GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) * 2.f; // 156.f

	Start.Z += CapsuleHeight + diff;
	End.Z += CapsuleHeight + diff;

	FHitResult OutHit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	bool bResult = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, QueryParams);

	float InRate = 0.f;

	// Climbing 
	if (bResult) // Climbing 
	{
		if (!HasAuthority())
		{
			ServerPressClimbingUp(ClimbingUp_Anim, true);
		}

		InRate = 1.3f;
		CanPressClimbingUp = false;
		bIsOnWall = true;
		bIsClimbingUp = true;

		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		MulticastPlayAnimation(ClimbingUp_Anim);

	}

	// Complete
	else
	{
		if (bIsOnWall) // Climbing 완료
		{
			if (!HasAuthority())
			{
				ServerPressClimbingUp(ClimbingComplete_Anim, false);
			}

			InRate = 1.1f;
			MulticastPlayAnimation(ClimbingComplete_Anim);
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			bIsOnWall = false;
			bIsClimbingUp = false;
			bIsClimbingComplete = true;


		}
	}

	GetWorld()->GetTimerManager().SetTimer(ClimbingTimerHandle, this, &AActionGameCharacter::ReleaseClimbing, InRate, false);
}

void AActionGameCharacter::ReleaseClimbing()
{
	CanPressClimbingUp = true;
	bIsClimbingUp = false;
	bIsClimbingComplete = false;

	GetWorld()->GetTimerManager().ClearTimer(ClimbingTimerHandle);
}

void AActionGameCharacter::ServerPressClimbingUp_Implementation(UAnimMontage* AnimMontage, bool flag)
{
	PressClimbingUp();
	bIsOnWall = flag;
	bIsClimbingUp = flag;
	MulticastPlayAnimation(AnimMontage);
}


// Inventory

void AActionGameCharacter::Interact()
{
	// 'Z"키를 눌러 아이템을 수집할 수 있습니다.
	if (CurrentInteractable != nullptr)
	{
		CurrentInteractable->OnPickedUp();
		AddItemToInventory(CurrentInteractable);
	}

}

void AActionGameCharacter::AddItemToInventory(class AItem* Item)
{
	if (Item != NULL)
	{
		Inventory->AddItem(Item); // 인벤토리에 아이템을 추가합니다. 
		CurrentInteractable = nullptr;
	}
}

void AActionGameCharacter::CheckForInteractable(class AItem* Item)
{
	//if (HasAuthority())
	//{
	if (Item == NULL)
	{
		CurrentInteractable = nullptr;
		return;
	}
	CurrentInteractable = Item;
	CurrentInteractable->SetOwner(this);
	//CurrentInteractable->SetOwningPawn(this);
	//}

	// 현재 Overlap된 아이템이 존재한다면 CurrentInteractable에 넣어줍니다. 
}

void AActionGameCharacter::UseItem(class AItem* Item)
{
	if (!HasAuthority())
	{
		ServerUseItem(Item);
	}
	Item->Use(); // Use 함수를 호출하여 각 아이템은 구현된 기능을 합니다. 
	Inventory->RemoveItem(Item); // 인벤토리에서 아이템을 제거합니다. 
}

void AActionGameCharacter::ServerUseItem_Implementation(class AItem* Item)
{
	Item->Use();
}

void AActionGameCharacter::SelectInventory()
{
	AActionPlayerController* PlayerController = Cast<AActionPlayerController>(GetController());
	// '탭'키를 누르면 인벤토리 창을 보여줍니다. 
	if (PlayerController->bOnInventoryHUD) // 이미 인벤토리 창이 화면에 존재한다면 
	{
		PlayerController->RemoveHUD(EHUDState::Inventory);
		// 인벤토리 창을 제거해줍니다. 
	}
	else
	{
		PlayerController->ChangeHUDState(EHUDState::Inventory);
		// 인벤토리 창이 존재하지 않았다면 추가해줍니다. 
	}

}

// Player State Interface
UStatComponent* AActionGameCharacter::GetMyStatComponent()
{
	if (Stat) { return Stat; }
	return NewObject<UStatComponent>();
}
UInventoryComponent* AActionGameCharacter::GetMyInventoryComponent()
{
	if (Inventory) { return Inventory; }
	return NewObject<UInventoryComponent>();
}
bool AActionGameCharacter::GetIsDeath() { return bIsDeath; }
int32 AActionGameCharacter::GetCurrentWeaponIndex() { return CurrentWeaponIndex; }
bool AActionGameCharacter::GetIsOnWall() { return bIsOnWall; }
bool AActionGameCharacter::GetIsClibmingUp() { return bIsClimbingUp; }

void AActionGameCharacter::SetSkillWidget()
{
	AActionPlayerController* PlayerController = Cast<AActionPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController)
	{
		SkillWidget = PlayerController->SkillWidget;
	}
}

void AActionGameCharacter::EnableMouseCursor()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	PlayerController->SetShowMouseCursor(true);
}

void AActionGameCharacter::DisableMouseCursor()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	PlayerController->SetShowMouseCursor(false);
}

void AActionGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AActionGameCharacter, CurrentWeapon);
	DOREPLIFETIME(AActionGameCharacter, bIsDeath);
	DOREPLIFETIME(AActionGameCharacter, bIsOnWall);
	//DOREPLIFETIME(AActionGameCharacter, bIsClimbingUp);

	DOREPLIFETIME(AActionGameCharacter, AttackIndex);
	DOREPLIFETIME(AActionGameCharacter, HangingLocation);



}