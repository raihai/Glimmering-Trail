// Fill out your copyright notice in the Description page of Project Settings.


#include "AirborneState.h"

void UAirborneState::HandleJump()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "In air ");
}
void UAirborneState::OnEnterState(AActor* OwnerRef)
{
	Super::OnEnterState(OwnerRef);

}


void UAirborneState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);
	
	//PlayerRef->PlayerMoveComponent->HandleJumping(DeltaTime); //

	//float gravity = FMath::Abs(GetWorld()->GetGravityZ());

	//******************** early jump termination needs to be added

	float gravity = (2 * 300) / (0.44 * 0.44);

	FVector currVelocity = PlayerRef->PlayerMoveComponent->Velocity;
	currVelocity.Z -= gravity * DeltaTime;

	PlayerRef->PlayerMoveComponent->Velocity = currVelocity;
	
	FVector Translation = currVelocity * DeltaTime;
	FHitResult hitResult;
	PlayerRef->AddActorWorldOffset(Translation, true, &hitResult);

	if (hitResult.IsValidBlockingHit()) {
		
		
		PlayerRef->StateManager->SwitchStateByKey("Grounded");

	}

}
