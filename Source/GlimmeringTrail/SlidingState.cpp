// Fill out your copyright notice in the Description page of Project Settings.


#include "SlidingState.h"

void USlidingState::OnEnterState(AActor* OwnerRef)
{
	Super::OnEnterState(OwnerRef);
	UE_LOG(LogTemp, Warning, TEXT("Entering Sliding State"));
}

void USlidingState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);

	FHitResult hitResult;
	IsGroundedSlide(hitResult);
	FVector surfaceNormal = hitResult.ImpactNormal;

	if (!SlopeCheck(surfaceNormal) ) {
		PlayerRef->PlayerMoveComponent->Velocity = FVector::ZeroVector;
		PlayerRef->StateManager->SwitchStateByKey("Grounded");
		return;
	}

	MovePlayerAlongSlope(surfaceNormal, hitResult, DeltaTime);
}

void USlidingState::OnExitState()
{
	Super::OnExitState();
	UE_LOG(LogTemp, Warning, TEXT("Slide State leaving Velocity: %s "), *PlayerRef->PlayerMoveComponent->Velocity.ToString());
}

void USlidingState::MovePlayerAlongSlope(const FVector& SurfaceNormal, FHitResult& HitRes, float deltaTime)
{
	FVector playerUpVec = PlayerRef->GetActorUpVector();
	FVector slideDirection = playerUpVec - (FVector::DotProduct(playerUpVec, SurfaceNormal) / (SurfaceNormal.Size() * SurfaceNormal.Size())) * SurfaceNormal; // vector projection onto plane
	slideDirection.Normalize();

	FVector slopeRightVec = FVector::CrossProduct(SurfaceNormal, slideDirection).GetSafeNormal(); // Due to the right handness nature of cross product, when we a cross product on the opposite slope
	float d = (float)FVector::DotProduct(PlayerRef->GetActorForwardVector(), slopeRightVec);

	if (d < 0) slopeRightVec = -slopeRightVec;

	FVector downwardForce;
	if (PlayerController->CurrentFrontBackValue) downwardForce = slideDirection * -400.0f;
	else downwardForce = slideDirection * -1000.0f;

	FVector inputMovement = slopeRightVec * PlayerController->CurrentFrontBackValue * 800.0f;
	FVector totalMovement = downwardForce + inputMovement;
	totalMovement *= deltaTime;

	PlayerRef->PlayerMoveComponent->Velocity = totalMovement;
	PlayerRef->AddActorWorldOffset(totalMovement, true, &HitRes);
}

