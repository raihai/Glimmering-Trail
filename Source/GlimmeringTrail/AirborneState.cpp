// Fill out your copyright notice in the Description page of Project Settings.


#include "AirborneState.h"

void UAirborneState::PressJump()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "IN air ");
}

void UAirborneState::TickState()
{
	Super::TickState();
	PlayerRef->StateManager->SwitchStateByKey("Air");
}
