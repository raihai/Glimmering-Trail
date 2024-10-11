// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleState.h"
#include "PlayerMovementComponent.h"

void UIdleState::HandleJump()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Purple, "Jumping from idle state");
	//PlayerRef->PlayerMoveComponent->HandleJumping();

}

void UIdleState::OnEnterState(AActor* OwnerRef)
{
	Super::OnEnterState(OwnerRef);

	// upon entering, change the velocity


	//PlayerRef->PlayerMoveComponent->Velocity *= FVector(0, 0, 1);
}


void UIdleState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);
	if (!PlayerRef->IsGrounded()) {
		PlayerRef->StateManager->SwitchStateByKey("Air");
	}

	// Add Switch to Walking State

	//if(PlayerRef->)

	//Add Switch to Running States;
}


