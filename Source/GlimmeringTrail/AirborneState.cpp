// Fill out your copyright notice in the Description page of Project Settings.


#include "AirborneState.h"

void UAirborneState::HandleJump()
{
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "In air ");
}

void UAirborneState::OnEnterState(AActor* OwnerRef)
{
    Super::OnEnterState(OwnerRef);
    UE_LOG(LogTemp, Warning, TEXT("Entering Airborne State"));
    bIsFallingFromLedge = (PlayerRef->PlayerMoveComponent->Velocity.Z == 0.0f); // Assume ledge fall if no upward velocity // fix

    m_Velocity = PlayerRef->PlayerMoveComponent->Velocity;
   
}

void UAirborneState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);
	//******************** early jump termination needs to be added

    FallingMovement(DeltaTime);
}



void UAirborneState::OnExitState()
{
	Super::OnExitState();
    bIsFallingFromLedge = false;
}

void UAirborneState::IsFallingFromLedge(float deltaTime)
{
    if (bIsFallingFromLedge) { // off ledge

        UE_LOG(LogTemp, Warning, TEXT("Falling OFF lEDGE"));
        float reducedGravity = gravity * 1.3f;
        m_Velocity.Z -= reducedGravity * deltaTime;
        m_Velocity += PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 35;
        m_Velocity += PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 35;
    }
    else { // regular

        UE_LOG(LogTemp, Warning, TEXT("Falling  Regular"));
        m_Velocity.Z -= gravity * deltaTime;
        m_Velocity += PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 15;
        m_Velocity += PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 15;
    }
}

void UAirborneState::FallingMovement(float DeltaTime)
{
    m_Velocity = PlayerRef->PlayerMoveComponent->Velocity;
    IsFallingFromLedge(DeltaTime);
    PlayerRef->PlayerMoveComponent->Velocity = m_Velocity;

    FVector Translation = m_Velocity * DeltaTime;
    FHitResult hitResult;
    PlayerRef->AddActorWorldOffset(Translation, true, &hitResult);

    if (hitResult.IsValidBlockingHit() && IsGrounded(hitResult))
    {
        PlayerRef->StateManager->SwitchStateByKey("Grounded");
    }

}
