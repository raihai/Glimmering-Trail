// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformPlayerController.h"

void APlatformPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
}

void APlatformPlayerController::PressJump()
{
	if (JumpDelegate.IsBound()) {
		JumpDelegate.Broadcast();
	}
}

void APlatformPlayerController::BeginPlay()
{
}

FJUMPSIGNNATURE* APlatformPlayerController::GetJumpDelegate()
{
	return &JumpDelegate;
}
