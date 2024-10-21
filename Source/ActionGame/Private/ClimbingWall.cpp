// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbingWall.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


AClimbingWall::AClimbingWall()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->SetBoxExtent(FVector(10.f, 100.f, 100.f));
	CollisionComp->SetCollisionProfileName(FName("BlockAllDynamic"));
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	//CollisionComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

}

// Called when the game starts or when spawned
void AClimbingWall::BeginPlay()
{
	Super::BeginPlay();

}

