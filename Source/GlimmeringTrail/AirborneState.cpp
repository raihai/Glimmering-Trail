// Fill out your copyright notice in the Description page of Project Settings.


#include "AirborneState.h"

void UAirborneState::HandleJump()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "In air ");
}

void UAirborneState::OnEnterState(AActor* OwnerRef)
{
	Super::OnEnterState(OwnerRef);
	
	// Inherit horizontal velocity from the previous state
	FVector currentVelocity = PlayerRef->PlayerMoveComponent->Velocity;

	// Maintain horizontal velocity (X and Y) while applying vertical velocity (Z)
	if (currentVelocity.Z == 0)
	{
		currentVelocity.Z = FMath::Sqrt(2 * 300 *gravity);
	}

	PlayerRef->PlayerMoveComponent->Velocity = currentVelocity;

	UE_LOG(LogTemp, Warning, TEXT("Airborne State Entered: Inherited Velocity = %s"), *currentVelocity.ToString());
	
}

void UAirborneState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);
	//******************** early jump termination needs to be added

	FVector currVelocity = PlayerRef->PlayerMoveComponent->Velocity;
	currVelocity += (PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 20) + (PlayerRef->GetActorRightVector() *PlayerController->CurrentSideValue * 20);
	currVelocity.Z -= gravity * DeltaTime; // apply gravity

	PlayerRef->PlayerMoveComponent->Velocity = currVelocity; // updating curr palyer velocity
	
	//apply movement
	FVector Translation = currVelocity * DeltaTime;
	FHitResult hitResult;
	PlayerRef->AddActorWorldOffset(Translation, true, &hitResult);

	if (hitResult.IsValidBlockingHit()) {
		PlayerRef->StateManager->SwitchStateByKey("Grounded");
	}
}



void UAirborneState::OnExitState()
{
	Super::OnExitState();
	/*UE_LOG(LogTemp, Warning, TEXT("Exiting AIRBORNE State: FrontValue %f"), FrontBackValue);
	UE_LOG(LogTemp, Warning, TEXT("Exiting AIRBONE State: SideValue %f"), SideValue);*/
}
