// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "Enemies/Enemy.h"
#include "AI/AIController_Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"


UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;

}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemy* Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());

	APawn* PlayerPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	//IPlayerStateInterface* PlayerState = Cast<IPlayerStateInterface>(PlayerPawn);

	if (Enemy == nullptr || PlayerPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), PlayerPawn->GetActorLocation());
	Enemy->SetActorRotation(NewRotation);
	Enemy->Attack();

	bIsAttacking = true;

	Enemy->OnAttackEnd.AddLambda([this]()
		{
			bIsAttacking = false;
		});

	return Result;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (bIsAttacking == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
