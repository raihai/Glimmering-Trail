// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleState.h"
#include "PlayerMovementComponent.h"


void UIdleState::OnEnterState(AActor* OwnerRef)
{
	Super::OnEnterState(OwnerRef);
}

void UIdleState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);
	
	if (PlayerController->CurrentFrontBackValue != 0 || PlayerController->CurrentSideValue != 0) {

		PlayerRef->StateManager->SwitchStateByKey("Walking");
	}
}

void UIdleState::OnExitState()
{

	Super::OnExitState();
	
}


