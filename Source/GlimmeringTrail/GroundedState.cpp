// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundedState.h"

void UGroundedState::HandleJump()
{
	// jumping from grounded state

	bIsJumping = true;

	//float g = FMath::Abs(GetWorld()->GetGravityZ());
	
	float g = (2 * 300) / (0.44 * 0.44);
	float jumpHeight = 300.0f;

	float jumpVelocity = sqrt(2 * g * jumpHeight);

	FVector currVelocity = PlayerRef->PlayerMoveComponent->Velocity;
	currVelocity.Z = jumpVelocity;

	PlayerRef->PlayerMoveComponent->Velocity = currVelocity;

}

void UGroundedState::OnEnterState(AActor* OwnerRef)
{
	Super::OnEnterState(OwnerRef);



}

void UGroundedState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);
	
	if (bIsJumping) {
		PlayerRef->StateManager->SwitchStateByKey("Air");
		bIsJumping = false;
	}
	else {

		PlayerRef->StateManager->SwitchStateByKey("Walking");

	}

	// if
	
}

void UGroundedState::OnExitState()
{
	Super::OnExitState();


}
