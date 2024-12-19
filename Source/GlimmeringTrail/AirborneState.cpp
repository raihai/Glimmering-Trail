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

    m_LedgeNormal = FVector::ZeroVector;
    FHitResult GroundHit;
    bIsFallingFromLedge = OnEdgeCheck(GroundHit);// Check if PLAYER in edge
    bSlopeEdge = IsGroundedOnSlope(GroundHit); // check if player is on slope
    m_Velocity = PlayerRef->PlayerMoveComponent->Velocity;
    UE_LOG(LogTemp, Warning, TEXT("is falling from ledge: %s"), bIsFallingFromLedge ? TEXT("True") : TEXT("False"));

}

void UAirborneState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);
	//******************** early jump termination needs to be added

    FHitResult hitResult;
   
    if (bIsFallingFromLedge && !bSlopeEdge) {
        IsFallingFromLedge(hitResult, DeltaTime);
    }
    else {
        FallingMovement(hitResult, DeltaTime);
    }
    
   // UE_LOG(LogTemp, Warning, TEXT("Current Velocity: %s"), *PlayerRef->PlayerMoveComponent->Velocity.ToString());

   if (IsGrounded(hitResult) || IsGroundedSlide(hitResult)) {
        PlayerRef->StateManager->SwitchStateByKey("Grounded");
    }
}

void UAirborneState::OnExitState()
{
	Super::OnExitState();
    bIsFallingFromLedge = false;
    bSlopeEdge = false;
    m_LedgeNormal = FVector::ZeroVector;
}

void UAirborneState::IsFallingFromLedge(FHitResult& hitResult, float deltaTime)
{
    UE_LOG(LogTemp, Warning, TEXT("Falling from ledge"));
    m_Velocity = PlayerRef->PlayerMoveComponent->Velocity;
    m_Velocity.Z -=  gravity * 1.2f * deltaTime;
     
    if (FMath::Abs(PlayerController->CurrentFrontBackValue) == FMath::Abs(PlayerController->CurrentSideValue)) {
        m_Velocity += PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 15.0f;
        m_Velocity += PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 15.0f;

    }
    else {
        m_Velocity += PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 30.0f;
        m_Velocity += PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 30.0f;
    }
  
    FVector outwardForce = m_LedgeNormal * 20000.0f; // Push outward from edge

    UE_LOG(LogTemp, Warning, TEXT("nomral vector : %s"), *m_LedgeNormal.ToString());
    m_Velocity += outwardForce * deltaTime;


    ClampHorizontalVelocity(m_Velocity, maxHorizontalHeight);
    FVector Translation = m_Velocity * deltaTime;
    PlayerRef->AddActorWorldOffset(Translation, true, &hitResult);

    if (hitResult.IsValidBlockingHit()) {
        FVector wallNormal = hitResult.ImpactNormal.GetSafeNormal();
        m_Velocity = FVector::VectorPlaneProject(m_Velocity, wallNormal);
        m_Velocity += wallNormal * 20.0f;
        UE_LOG(LogTemp, Warning, TEXT("Wall collision detected, velocity adjusted."));
    }

    PlayerRef->PlayerMoveComponent->Velocity = m_Velocity;
}

void UAirborneState::FallingMovement(FHitResult& hitResult, float DeltaTime){
    
    UE_LOG(LogTemp, Warning, TEXT("Falling Normally"));

    m_Velocity = PlayerRef->PlayerMoveComponent->Velocity;
    m_Velocity.Z -= gravity * DeltaTime;
  
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
        m_Velocity = FVector::VectorPlaneProject(m_Velocity, wallNormal);
        m_Velocity += wallNormal * 20.0f;
        UE_LOG(LogTemp, Warning, TEXT("Wall collision detected, velocity adjusted."));
    }

    PlayerRef->PlayerMoveComponent->Velocity = m_Velocity;
}

bool UAirborneState::OnEdgeCheck(FHitResult& hitres/*, float DeltaTime*/)
{
  
    FVector belowPlayerLocation = PlayerRef->GetActorLocation() - FVector(0, 0, 100.0f); // Bottom of capsule
    FVector offsets[] = {
        FVector(65, 0, 0), 
        FVector(-65, 0, 0), 
        FVector(0, 65, 0), 
        FVector(0, -65, 0)
    };
  
    bool bHits[4] = { false, false, false, false };
    bool bDHits[4] = { false, false, false, false };

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(PlayerRef);

    int missedRays = 0;
    int missedDownRays = 0;

    for (int32 i = 0; i < 4; ++i){
        FVector horizontalEnd = belowPlayerLocation + offsets[i];
        FVector downwardEnd = horizontalEnd - FVector(0, 0, 100.0f);

        bHits[i] = GetWorld()->LineTraceSingleByChannel(hitres, belowPlayerLocation, horizontalEnd, ECC_Visibility, CollisionParams);
        DrawDebugLine(GetWorld(), belowPlayerLocation, horizontalEnd, bHits[i] ? FColor::Green : FColor::Purple, false, 10.0f, 0, 1.0f);

        bDHits[i] = GetWorld()->LineTraceSingleByChannel(hitres, horizontalEnd, downwardEnd, ECC_Visibility, CollisionParams);
        DrawDebugLine(GetWorld(), horizontalEnd, downwardEnd, bDHits[i] ? FColor::Green : FColor::Purple, false, 10.0f, 0, 1.0f);

        if (!bHits[i]) missedRays += 1;
        if (!bDHits[i]) missedDownRays += 1;
    }

    if ((bHits[0] && bHits[1]) || (bHits[2] && bHits[3])){
        return false; 
    }

    m_LedgeNormal = FVector::ZeroVector;
    for (int32 i = 0; i < 4; ++i) {
        if (!bHits[i]) { m_LedgeNormal += offsets[i]; }
    }
    m_LedgeNormal.Normalize();

    // Final edge detection logic
    if (missedRays >= 3 && missedDownRays >= 3)
    {
        return true;
    }

    return false;

}

void UAirborneState::ClampHorizontalVelocity(FVector& Velocity, float MaxHorizontalSpeed)
{
    FVector HorizontalVelocity = FVector(Velocity.X, Velocity.Y, 0.0f); // Extract horizontal components
    float HorizontalSpeed = HorizontalVelocity.Size(); // magnitude

    if (HorizontalSpeed > MaxHorizontalSpeed)
    {
        HorizontalVelocity = HorizontalVelocity.GetSafeNormal() * MaxHorizontalSpeed; // Scale down to max speed
        Velocity.X = HorizontalVelocity.X;
        Velocity.Y = HorizontalVelocity.Y;
    }
}
