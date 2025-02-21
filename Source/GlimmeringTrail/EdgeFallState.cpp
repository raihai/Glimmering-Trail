// Fill out your copyright notice in the Description page of Project Settings.


#include "EdgeFallState.h"

void UEdgeFallState::HandleJump()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Cannot double jump");
}

void UEdgeFallState::OnEnterState(AActor* OwnerRef)
{
	Super::OnEnterState(OwnerRef);

    UE_LOG(LogTemp, Warning, TEXT("Entering EdgeFall State"));
    m_Velocity = PlayerRef->PlayerMoveComponent->Velocity;
}

void UEdgeFallState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);

    FHitResult fHit;

  
    FallingFromLedge(fHit, DeltaTime);
    
    if (IsGroundedRay(fHit)) {
        PlayerRef->StateManager->SwitchStateByKey("Grounded");
    }
}

void UEdgeFallState::OnExitState()
{
	Super::OnExitState();
    UE_LOG(LogTemp, Warning, TEXT("Leaving EdgeFall State"));

    bIsFallingFromLedge = false;
    bSlopeEdge = false;
    m_LedgeNormal = FVector::ZeroVector;
    
}

void UEdgeFallState::FallingFromLedge(FHitResult& hitResult, float deltaTime)
{
    UE_LOG(LogTemp, Warning, TEXT("Falling from ledge"));
    m_Velocity = PlayerRef->PlayerMoveComponent->Velocity;
    m_Velocity.Z -= gravity * 1.2f * deltaTime;
    if (FMath::IsNearlyZero(PlayerController->CurrentFrontBackValue) && FMath::IsNearlyZero(PlayerController->CurrentSideValue))return;

    FVector inputMovement = FVector::ZeroVector;
    inputMovement += PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 30.0f;
    inputMovement += PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 30.0f;

    FVector slideDirection = m_LedgeNormal;
    if (!m_LedgeNormal.IsNearlyZero() && bSlopeEdge) {
        FVector slopeNormal = hitResult.ImpactNormal.GetSafeNormal();
        slideDirection = FVector::VectorPlaneProject(m_LedgeNormal, slopeNormal);
        slideDirection.Normalize();
    }

    m_Velocity += (slideDirection * 200.0f + inputMovement) * deltaTime;

    ClampHorizontalVelocity(m_Velocity, maxHorizontalHeight);
    FVector Translation = m_Velocity * deltaTime;
    PlayerRef->AddActorWorldOffset(Translation, true, &hitResult);

    if (hitResult.IsValidBlockingHit()) {
        FVector wallNormal = hitResult.ImpactNormal.GetSafeNormal();
        m_Velocity = FVector::VectorPlaneProject(m_Velocity, wallNormal);
        UE_LOG(LogTemp, Warning, TEXT("Wall collision detected, velocity adjusted."));
    }

    PlayerRef->PlayerMoveComponent->Velocity = m_Velocity;


    //m_Velocity = PlayerRef->PlayerMoveComponent->Velocity;
    //m_Velocity.Z -= gravity * DeltaTime;
    ////  if (FMath::IsNearlyZero(PlayerController->CurrentFrontBackValue) && FMath::IsNearlyZero(PlayerController->CurrentSideValue))return;

    //if (FMath::Abs(PlayerController->CurrentFrontBackValue) == FMath::Abs(PlayerController->CurrentSideValue)) {
    //    m_Velocity += PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 7.5f;
    //    m_Velocity += PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 7.5f;
    //}
    //else {
    //    m_Velocity += PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 15.0f;
    //    m_Velocity += PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 15.0f;
    //}

    //ClampHorizontalVelocity(m_Velocity, maxHorizontalHeight);
    //FVector Translation = m_Velocity * DeltaTime;
    //PlayerRef->AddActorWorldOffset(Translation, true, &hitResult);

    //if (hitResult.IsValidBlockingHit()) {
    //    FVector wallNormal = hitResult.ImpactNormal.GetSafeNormal();
    //    m_Velocity = FVector::VectorPlaneProject(m_Velocity, wallNormal).GetSafeNormal();
    //    // m_Velocity += wallNormal * 20.0f;
    //    UE_LOG(LogTemp, Warning, TEXT("Wall collision detected, velocity adjusted."));
    //}

    //PlayerRef->PlayerMoveComponent->Velocity = m_Velocity;


}


