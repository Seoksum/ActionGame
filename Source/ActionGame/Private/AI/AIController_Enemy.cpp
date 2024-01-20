// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIController_Enemy.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/Enemy.h"
#include "Enemies/Enemy_Boss.h"


const FName AAIController_Enemy::HomePosKey(TEXT("HomePos"));
const FName AAIController_Enemy::PatrolPosKey(TEXT("PatrolPos"));

AAIController_Enemy::AAIController_Enemy()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/AI/BT_Enemy.BT_Enemy'"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("BlackboardData'/Game/AI/BB_Enemy.BB_Enemy'"));
	if (BD.Succeeded())
	{
		BlackboardData = BD.Object;
	}
}

void AAIController_Enemy::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BlackboardData, Blackboard))
	{
		AEnemy* Enemy = Cast<AEnemy>(InPawn);

		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		Blackboard->SetValueAsObject(FName(TEXT("Enemy")), Enemy);

		if (RunBehaviorTree(BehaviorTree))
		{
			// TODO

		}
	}
}

void AAIController_Enemy::OnUnPossess()
{
	Super::OnUnPossess();

}

bool AAIController_Enemy::IsDead() const
{
	AEnemy* Enemy = Cast<AEnemy>(GetPawn());
	if (Enemy)
	{
		return Enemy->IsDeath;
		//return Enemy->IsPawnControlled();
	}

	return false; // AI가 죽으면 폰이 AI 컨트롤러와 분리됨
}

bool AAIController_Enemy::IsBossDead() const
{
	//TODO
	//IAIEnemyInterface* BossEnemy = Cast<IAIEnemyInterface>(GetPawn());
	//if (BossEnemy)
	//{
	//	return BossEnemy->IsBossEnemyDead();
	//}

	AEnemy_Boss* BossEnemy = Cast<AEnemy_Boss>(GetPawn());
	if (BossEnemy)
	{
		return BossEnemy->IsPawnControlled();
	}

	return false; // AI가 죽으면 폰이 AI 컨트롤러와 분리됨
}

