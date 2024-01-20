// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Weapon.h"
#include "Characters/ActionGameCharacter.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AWeapon::AWeapon()
{
 	
	//SetReplicates(true);
	bReplicates = true;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	OnRep_MyPawn();
}

void AWeapon::SetOwningPawn(AActionGameCharacter* NewOwner)
{
	if (NewOwner)
	{
		SetInstigator(NewOwner);
		MyPawn = NewOwner;
		SetOwner(NewOwner);
	}
}

void AWeapon::OnRep_MyPawn()
{
	MyPawn = Cast<AActionGameCharacter>(GetOwner());
	if (MyPawn)
	{
		SetOwningPawn(MyPawn);
	}
	else
	{
		SetOwningPawn(nullptr);
	}
}

void AWeapon::Attack()
{

}


void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, MyPawn);

}