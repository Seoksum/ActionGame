// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class ACTIONGAME_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	void SetOwningPawn(class AActionGameCharacter* NewOwner);

	UPROPERTY(Transient, ReplicatedUsing = OnRep_MyPawn)
	class AActionGameCharacter* MyPawn;

	UFUNCTION()
	void OnRep_MyPawn();

	virtual void Attack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




};
