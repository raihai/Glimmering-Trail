// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleState.h"

void UIdleState::PressJump()
{
	//PlayerRef->DoubleJump();

}

void UIdleState::TickState()
{

	PlayerRef->StateManager->SwitchStateByKey("Air");
}
