

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
	Inventory,	// 인벤토리 UI
	LoadingScreen,
	WinScreen,
	LoseScreen,
};