// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleState.h"

void UIdleState::PressJump()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Purple, "Idle state now");
}

void UIdleState::TickState()
{

	if (!PlayerRef->IsGrounded()) {
		PlayerRef->StateManager->SwitchStateByKey("air");
	}
	
}
