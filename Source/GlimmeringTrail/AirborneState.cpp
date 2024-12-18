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
    bIsFallingFromLedge = OnEdgeCheck(GroundHit);// Check if PLAYER in on a slope 
    m_Velocity = PlayerRef->PlayerMoveComponent->Velocity;
   
}

void UAirborneState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);
	//******************** early jump termination needs to be added

    FHitResult hitResult;

    if (bIsFallingFromLedge) {
        IsFallingFromLedge(hitResult, DeltaTime);
    }
    else {
        FallingMovement(hitResult, DeltaTime);
    }
    

   // UE_LOG(LogTemp, Warning, TEXT("Current Velocity: %s"), *PlayerRef->PlayerMoveComponent->Velocity.ToString());

   if (IsGrounded(hitResult)) {
        PlayerRef->StateManager->SwitchStateByKey("Grounded");
    }


}

void UAirborneState::OnExitState()
{
	Super::OnExitState();
    bIsFallingFromLedge = false;
}

void UAirborneState::IsFallingFromLedge(FHitResult& hitResult, float deltaTime)
{

    UE_LOG(LogTemp, Warning, TEXT("Falling from ledge"));
    m_Velocity = PlayerRef->PlayerMoveComponent->Velocity;
    m_Velocity.Z -=  gravity * 1.3f * deltaTime;

 //   m_Velocity += PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 25.0f;
   // m_Velocity += PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 25.0f;

    FVector outwardForce = m_LedgeNormal * 2000.0f; // Push outward from edge
    m_Velocity += outwardForce * deltaTime;

    FVector Translation = m_Velocity * deltaTime;
    PlayerRef->AddActorWorldOffset(Translation, true, &hitResult);

    if (hitResult.IsValidBlockingHit()) {
        FVector wallNormal = hitResult.ImpactNormal.GetSafeNormal();
        m_Velocity = FVector::VectorPlaneProject(m_Velocity, wallNormal);
        m_Velocity += wallNormal * 10.0f;
        UE_LOG(LogTemp, Warning, TEXT("Wall collision detected, velocity adjusted."));
    }

    PlayerRef->PlayerMoveComponent->Velocity = m_Velocity;
   // UE_LOG(LogTemp, Warning, TEXT("Moving Player on Edge: Velocity: %s"), *m_Velocity.ToString());
}

void UAirborneState::FallingMovement(FHitResult& hitResult, float DeltaTime){
  
    m_Velocity = PlayerRef->PlayerMoveComponent->Velocity;
    m_Velocity.Z -= (bIsFallingFromLedge ? gravity * 1.3f : gravity) * DeltaTime;
  
    m_Velocity += PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 15.0f;
    m_Velocity += PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 15.0f;
  
    FVector Translation = m_Velocity * DeltaTime;
    PlayerRef->AddActorWorldOffset(Translation, true, &hitResult);

    if (hitResult.IsValidBlockingHit()){
        FVector wallNormal = hitResult.ImpactNormal.GetSafeNormal();
        m_Velocity = FVector::VectorPlaneProject(m_Velocity, wallNormal);
        m_Velocity += wallNormal * 10.0f;
        UE_LOG(LogTemp, Warning, TEXT("Wall collision detected, velocity adjusted."));
    }

    PlayerRef->PlayerMoveComponent->Velocity = m_Velocity;
}

bool UAirborneState::OnEdgeCheck(FHitResult& hitres/*, float DeltaTime*/)
{
    FVector playerLocation = PlayerRef->GetActorLocation();

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(PlayerRef);

    FVector belowPlayerLocation = playerLocation - FVector(0, 0, 120.0f); // Bottom of capsule
    FVector offsets[] = {
        FVector(65, 0, -10),    // Forward
        FVector(-65, 0, -10),   // Backward
        FVector(0, 65, -10),    // Right
        FVector(0, -65, -10)    // Left
    };

   bool forwardHit = GetWorld()->LineTraceSingleByChannel(
       hitres,
       belowPlayerLocation,
       belowPlayerLocation  + offsets[0],
       ECC_Visibility,
       CollisionParams
   );

   bool backwardHit = GetWorld()->LineTraceSingleByChannel(
       hitres,
       belowPlayerLocation,
       belowPlayerLocation + offsets[1],
       ECC_Visibility,
       CollisionParams
   );
   bool rightHit = GetWorld()->LineTraceSingleByChannel(
       hitres,
       belowPlayerLocation,
       belowPlayerLocation + offsets[2],
       ECC_Visibility,
       CollisionParams
   );
    bool leftHit = GetWorld()->LineTraceSingleByChannel(
       hitres,
       belowPlayerLocation,
       belowPlayerLocation + offsets[3],
       ECC_Visibility,
       CollisionParams
   );

    DrawDebugLine(GetWorld(), belowPlayerLocation, belowPlayerLocation + offsets[0], forwardHit ? FColor::Green : FColor::Purple, false, 10.0f, 0, 1.0f);
    DrawDebugLine(GetWorld(), belowPlayerLocation, belowPlayerLocation + offsets[1], backwardHit ? FColor::Green : FColor::Purple, false, 10.0f, 0, 1.0f);
    DrawDebugLine(GetWorld(), belowPlayerLocation, belowPlayerLocation + offsets[2], rightHit ? FColor::Green : FColor::Purple, false, 10.0f, 0, 1.0f);
    DrawDebugLine(GetWorld(), belowPlayerLocation, belowPlayerLocation + offsets[3], leftHit ? FColor::Green : FColor::Purple, false, 10.0f, 0, 1.0f);


    if ((forwardHit && backwardHit) || (rightHit && leftHit))
    {
        return false; // opposite vectors hit
    }

    if (forwardHit && backwardHit && rightHit &&leftHit) {
        return false;
    }
    //hitnormmal 


    if (!forwardHit) {
        m_LedgeNormal += offsets[0];
    }
    if (!backwardHit) {
        m_LedgeNormal += offsets[1];
    }

    if (!rightHit) {
        m_LedgeNormal += offsets[2];
    }

    if (!leftHit) {
        m_LedgeNormal += offsets[3];
    }

    m_LedgeNormal.Normalize();
    return false; 
}
void UAirborneState::MoveOnEdgePlayer(FVector& normal, float DeltaTime)
{

    FVector outwardForce = normal * 200.0f; // Push outward from edge
    FVector downwardForce = FVector(0, 0, -gravity); // Apply stronger downward force

    m_Velocity += (outwardForce + downwardForce) * DeltaTime;

    UE_LOG(LogTemp, Warning, TEXT("Moving Player on Edge: Velocity: %s"), *m_Velocity.ToString());


}
