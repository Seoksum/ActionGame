// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_SearchTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Enemies/Enemy.h"
#include "Characters/ActionGameCharacter.h"

UBTService_SearchTarget::UBTService_SearchTarget()
{
	NodeName = TEXT("SearchTarget");
	Interval = 1.f;
}

void UBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	AEnemy* Enemy = Cast< AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr || Enemy->IsDeath)
	{
		return;
	}

	UWorld* World = Enemy->GetWorld();
	FVector Center = Enemy->GetActorLocation();
	float SearchRadius = 600.f;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, Enemy);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		FCollisionShape::MakeSphere(SearchRadius),
		QueryParams);

	if (bResult)
	{
		for (auto& OverlapResult : OverlapResults)
		{
			//APawn* PlayerPawn = Cast<APawn>(OverlapResult.GetActor());
			AActionGameCharacter* PlayerPawn = Cast<AActionGameCharacter>(OverlapResult.GetActor());
			if (PlayerPawn)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), PlayerPawn);
				return;
			}
			//else
			//{
			//	OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
			//}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
	}
}
