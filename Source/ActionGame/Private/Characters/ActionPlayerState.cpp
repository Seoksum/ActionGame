// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ActionPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "GameFrameworks/ActionGameState.h"



AActionPlayerState::AActionPlayerState()
{
	AActionGameState* GS = Cast<AActionGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->AddPlayerState(this);
	}
}