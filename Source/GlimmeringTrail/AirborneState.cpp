// Fill out your copyright notice in the Description page of Project Settings.


#include "AirborneState.h"

void UAirborneState::HandleJump()
{
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Cannot Not Double Jump");
}

void UAirborneState::OnEnterState(AActor* OwnerRef)
{
    Super::OnEnterState(OwnerRef);
    UE_LOG(LogTemp, Warning, TEXT("Entering Airborne State"));

    m_Velocity = PlayerRef->PlayerMoveComponent->Velocity;

    ElapsedTimeInAirborne = 0.0f;

}
void UAirborneState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);
	//******************** early jump termination needs to be added

   ElapsedTimeInAirborne += DeltaTime;

    FHitResult hitResult;

    if ( IsGrounded(hitResult)) {
        PlayerRef->StateManager->SwitchStateByKey("Grounded");
        return;
    }
    FallingMovement(hitResult, DeltaTime); 
}

void UAirborneState::OnExitState()
{
	Super::OnExitState();
    bIsFallingFromLedge = false;
    bSlopeEdge = false;
    m_LedgeNormal = FVector::ZeroVector;
    FallingFromLedge = false;
}

void UAirborneState::IsFallingFromLedge(FHitResult& hitResult, float deltaTime)
{
    //UE_LOG(LogTemp, Warning, TEXT("Falling from ledge"));
    //m_Velocity = PlayerRef->PlayerMoveComponent->Velocity;
    //m_Velocity.Z -=  gravity * 1.2f * deltaTime;
    // 
    //if (FMath::Abs(PlayerController->CurrentFrontBackValue) == FMath::Abs(PlayerController->CurrentSideValue)) {
    //    m_Velocity += PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 15.0f;
    //    m_Velocity += PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 15.0f;
    //}else {
    //    m_Velocity += PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 30.0f;
    //    m_Velocity += PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 30.0f;
    //}
  
    //if (!m_LedgeNormal.IsNearlyZero()) {
    //    FVector outwardForce = m_LedgeNormal * 500.0f;
    //    m_Velocity += outwardForce * deltaTime;
    //} // Push outward from edge

    //UE_LOG(LogTemp, Warning, TEXT("ledge fall, push off normal vector : %s"), *m_LedgeNormal.ToString());
    ////m_Velocity += outwardForce * deltaTime;

    //ClampHorizontalVelocity(m_Velocity, maxHorizontalHeight);
    //FVector Translation = m_Velocity * deltaTime;

    //PlayerRef->AddActorWorldOffset(Translation, true, &hitResult);

    //if (hitResult.IsValidBlockingHit()) {
    //    FVector wallNormal = hitResult.ImpactNormal.GetSafeNormal();
    //    m_Velocity = FVector::VectorPlaneProject(m_Velocity, wallNormal);
    //    m_Velocity += wallNormal * 20.0f;
    //    UE_LOG(LogTemp, Warning, TEXT("Wall collision detected, velocity adjusted."));
    //}

    //PlayerRef->PlayerMoveComponent->Velocity = m_Velocity;
    UE_LOG(LogTemp, Warning, TEXT("Falling from ledge"));
    m_Velocity = PlayerRef->PlayerMoveComponent->Velocity;
    m_Velocity.Z -= gravity * 1.2f * deltaTime;

    FVector inputMovement = FVector::ZeroVector;
    inputMovement += PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 30.0f;
    inputMovement += PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 30.0f;

    FVector slideDirection = m_LedgeNormal;
    if (!m_LedgeNormal.IsNearlyZero() && bSlopeEdge) {
        FVector slopeNormal = hitResult.ImpactNormal.GetSafeNormal();
        slideDirection = FVector::VectorPlaneProject(m_LedgeNormal, slopeNormal);
        slideDirection.Normalize();
    }

    m_Velocity += (slideDirection * 500.0f + inputMovement) * deltaTime;

    ClampHorizontalVelocity(m_Velocity, maxHorizontalHeight);
    FVector Translation = m_Velocity * deltaTime;
    PlayerRef->AddActorWorldOffset(Translation, true, &hitResult);

    if (hitResult.IsValidBlockingHit()) {
        FVector wallNormal = hitResult.ImpactNormal.GetSafeNormal();
        m_Velocity = FVector::VectorPlaneProject(m_Velocity, wallNormal);
        UE_LOG(LogTemp, Warning, TEXT("Wall collision detected, velocity adjusted."));
    }

    PlayerRef->PlayerMoveComponent->Velocity = m_Velocity;
}

void UAirborneState::FallingMovement(FHitResult& hitResult, float DeltaTime){
    
    UE_LOG(LogTemp, Warning, TEXT("Falling Normally"));

    m_Velocity = PlayerRef->PlayerMoveComponent->Velocity;
    m_Velocity.Z -= gravity * DeltaTime;
  //  if (FMath::IsNearlyZero(PlayerController->CurrentFrontBackValue) && FMath::IsNearlyZero(PlayerController->CurrentSideValue))return;
  
    if (FMath::Abs(PlayerController->CurrentFrontBackValue) == FMath::Abs(PlayerController->CurrentSideValue)) {
        m_Velocity += PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue *7.5f;
        m_Velocity += PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 7.5f;
    }
    else {
        m_Velocity += PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 15.0f;
        m_Velocity += PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 15.0f;
    }
    
    ClampHorizontalVelocity(m_Velocity, maxHorizontalHeight);
    FVector Translation = m_Velocity * DeltaTime;
    PlayerRef->AddActorWorldOffset(Translation, true, &hitResult);

    if (hitResult.IsValidBlockingHit()){
        FVector wallNormal = hitResult.ImpactNormal.GetSafeNormal();
        m_Velocity = FVector::VectorPlaneProject(m_Velocity, wallNormal).GetSafeNormal();
       // m_Velocity += wallNormal * 20.0f;
        UE_LOG(LogTemp, Warning, TEXT("Wall collision detected, velocity adjusted."));
    }

    PlayerRef->PlayerMoveComponent->Velocity = m_Velocity;
}

bool UAirborneState::OnEdgeCheck(FHitResult& hitres/*, float DeltaTime*/)
{
    
        FVector belowPlayerLocation = PlayerRef->GetActorLocation() - FVector(0, 0, 100.0f); // Bottom of capsule
        FVector offsets[] = {
            FVector(50, 0, 0),
            FVector(-50, 0, 0),
            FVector(0, 50, 0),
            FVector(0, -50, 0)
        };

        FCollisionQueryParams CollisionParams;
        CollisionParams.AddIgnoredActor(PlayerRef);

        int missedHorizontalRays = 0;
        int missedDownwardRays = 0;
        m_LedgeNormal = FVector::ZeroVector;

        for (const FVector& offset : offsets) {
            FVector horizontalEnd = belowPlayerLocation + offset;
            FVector downwardEnd = horizontalEnd - FVector(0, 0, 50.0f);

            bool bHorizontalHit = GetWorld()->LineTraceSingleByChannel(hitres, belowPlayerLocation, horizontalEnd, ECC_Visibility, CollisionParams);
            DrawDebugLine(GetWorld(), belowPlayerLocation, horizontalEnd, bHorizontalHit ? FColor::Green : FColor::Red, false, 1.0f, 0, 1.0f);

            bool bDownwardHit = GetWorld()->LineTraceSingleByChannel(hitres, horizontalEnd, downwardEnd, ECC_Visibility, CollisionParams);
            DrawDebugLine(GetWorld(), horizontalEnd, downwardEnd, bDownwardHit ? FColor::Green : FColor::Blue, false, 1.0f, 0, 1.0f);

            if (!bHorizontalHit) {
                missedHorizontalRays++;
                m_LedgeNormal += offset; // Accumulate normal from missed rays.
            }
            if (!bDownwardHit) missedDownwardRays++;
        }

        if (!m_LedgeNormal.IsNearlyZero()) {
            m_LedgeNormal.Normalize();
        }

        // Edge detection logic
        if ((missedHorizontalRays >= 2) && (missedDownwardRays >= 2)) {
            UE_LOG(LogTemp, Warning, TEXT("Detected ledge normal: %s"), *m_LedgeNormal.ToString());
            return true;
        }

        return false;
  


}

