// Fill out your copyright notice in the Description page of Project Settings.


#include "AirborneState.h"

void UAirborneState::PressJump()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "In air ");
}

void UAirborneState::TickState()
{
	Super::TickState();

	if (PlayerRef->IsGrounded()) {
		PlayerRef->StateManager->SwitchStateByKey("idle");
	}
	
}
