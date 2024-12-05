// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundedState.h"

void UGroundedState::HandleJump()
{
	// jumping from grounded state
	Super::HandleJump();
}

void UGroundedState::OnEnterState(AActor* OwnerRef)
{
	Super::OnEnterState(OwnerRef);

	if (!FMath::IsNearlyZero(PlayerController->CurrentFrontBackValue) || !FMath::IsNearlyZero(PlayerController->CurrentSideValue))
	{
		PlayerRef->StateManager->SwitchStateByKey("Walking");
	}

}

void UGroundedState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);
	
	if (!FMath::IsNearlyZero(PlayerController->CurrentFrontBackValue) && !FMath::IsNearlyZero(PlayerController->CurrentSideValue)) {
		PlayerRef->StateManager->SwitchStateByKey("Walking");
	}
	else {
		PlayerRef->StateManager->SwitchStateByKey("Idle");
	}

}


void UGroundedState::OnExitState()
{
	Super::OnExitState();
	
}
