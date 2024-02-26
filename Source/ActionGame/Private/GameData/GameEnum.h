

#pragma once

#include "CoreMinimal.h"
#include "GameEnum.generated.h"


UENUM()
enum ECharacterType
{
	Knight,
	Maze,
};

UENUM()
enum EEnemyType
{
	SkeletonEnemy,
	BossEnemy,
};

UENUM(BlueprintType)
enum class EHUDState : uint8
{
	Playing,
	MatchEnd,
	Inventory,	// �κ��丮 UI
	LoadingScreen,
	WinScreen,
	LoseScreen,
};