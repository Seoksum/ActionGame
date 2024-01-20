// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Characters/ActionGameCharacter.h"
#include "Interface/CharacterItemInterface.h"

#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "../ActionGame.h"


AItem::AItem()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	MeshComp->SetCollisionProfileName(FName("OverlapAll"));
	RootComponent = MeshComp;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnBeginOverlap);
	CollisionComp->InitSphereRadius(200.f);
	CollisionComp->SetupAttachment(MeshComp);
	CollisionComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f));


	ItemName = FText::FromString("Enter an item name here...");
	UseActionText = FText::FromString("Use");

	bReplicates = true;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	bIsMeshVisibility = true;
	MeshComp->SetVisibility(bIsMeshVisibility);
}

void AItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//AActionGameCharacter* Player = Cast<AActionGameCharacter>(OtherActor);
	ICharacterItemInterface* Player = Cast<ICharacterItemInterface>(OtherActor);

	if (Player)
	{
		Player->CheckForInteractable(this);
	}

}


void AItem::Use()
{
}

void AItem::OnPickedUp()
{
	NET_LOG(LogNetwork, Log, TEXT("%s"), TEXT("Begin"));

	if (HasAuthority())
	{
		bIsMeshVisibility = false;
		OnRep_MeshVisibility();
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//ServerOnPickedUp();
	}
	else
	{
		ServerOnPickedUp();
	}

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->SetCollisionProfileName(FName("NoCollision"));
	//MeshComp->SetVisibility(bIsMeshVisibility);
}

void AItem::ServerOnPickedUp_Implementation()
{
	OnPickedUp();
	bIsMeshVisibility = false;
	MeshComp->SetVisibility(bIsMeshVisibility);
}

void AItem::OnRep_MeshVisibility()
{
	MeshComp->SetVisibility(bIsMeshVisibility);
}

void AItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItem, bIsMeshVisibility);
	


}