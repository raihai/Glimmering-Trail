// Fill out your copyright notice in the Description page of Project Settings.


#include "AirborneState.h"

void UAirborneState::HandleJump()
{
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "In air ");
}

void UAirborneState::OnEnterState(AActor* OwnerRef)
{
	Super::OnEnterState(OwnerRef);
    bIsFallingFromLedge = (PlayerRef->PlayerMoveComponent->Velocity.Z == 0.0f); // Assume ledge fall if no upward velocity

    // Debug message
    if (bIsFallingFromLedge)
    {
        UE_LOG(LogTemp, Warning, TEXT("Falling from ledge: Enhanced air control active."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Jumping: Standard airborne behavior."));
    }
}

void UAirborneState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);
	//******************** early jump termination needs to be added

	//FVector currVelocity = PlayerRef->PlayerMoveComponent->Velocity;
	//currVelocity += (PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 20) + (PlayerRef->GetActorRightVector() *PlayerController->CurrentSideValue * 20);
	//currVelocity.Z -= gravity * DeltaTime; // apply gravity

	//PlayerRef->PlayerMoveComponent->Velocity = currVelocity; // updating curr palyer velocity
	//
	////apply movement
	//FVector Translation = currVelocity * DeltaTime;
	//FHitResult hitResult;
	//PlayerRef->AddActorWorldOffset(Translation, true, &hitResult);

	//if (hitResult.IsValidBlockingHit()) {
	//	PlayerRef->StateManager->SwitchStateByKey("Grounded");
	//}

    FVector currVelocity = PlayerRef->PlayerMoveComponent->Velocity;

    if (bIsFallingFromLedge)
    {
        // Reduced gravity for falling off ledges
        float reducedGravity = gravity * 1.3f;
        currVelocity.Z -= reducedGravity * DeltaTime;

        // Enhanced horizontal control
        currVelocity += PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 35;
        currVelocity += PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 35;
    }
    else
    {
        // Standard gravity for jumping
        currVelocity.Z -= gravity * DeltaTime;

        // Regular horizontal control for jumps
        currVelocity += PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 15;
        currVelocity += PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 15;
    }

    // Clamp horizontal speed for better control
    float maxHorizontalSpeed = bIsFallingFromLedge ? 600.0f : 500.0f;
    currVelocity.X = FMath::Clamp(currVelocity.X, -maxHorizontalSpeed, maxHorizontalSpeed);
    currVelocity.Y = FMath::Clamp(currVelocity.Y, -maxHorizontalSpeed, maxHorizontalSpeed);

    // Apply velocity to the player
    PlayerRef->PlayerMoveComponent->Velocity = currVelocity;

    FVector Translation = currVelocity * DeltaTime;
    FHitResult hitResult;
    PlayerRef->AddActorWorldOffset(Translation, true, &hitResult);

    if (hitResult.IsValidBlockingHit())
    {
        PlayerRef->StateManager->SwitchStateByKey("Grounded");
    }
}



void UAirborneState::OnExitState()
{
	Super::OnExitState();
    bIsFallingFromLedge = false;
	/*UE_LOG(LogTemp, Warning, TEXT("Exiting AIRBORNE State: FrontValue %f"), FrontBackValue);
	UE_LOG(LogTemp, Warning, TEXT("Exiting AIRBONE State: SideValue %f"), SideValue);*/
}
