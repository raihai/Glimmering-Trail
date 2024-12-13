// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingState.h"

void UWalkingState::OnEnterState(AActor* OwnerRef)
{
	Super::OnEnterState(OwnerRef);
	UE_LOG(LogTemp, Warning, TEXT("Entering WALKIng STATE"));
}

void UWalkingState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);

	FHitResult checkBelowPlayer;
	if (!IsGrounded(checkBelowPlayer)) {
		PlayerRef->StateManager->SwitchStateByKey("Air");
		return;
	}

	if (PlayerController->CurrentFrontBackValue == 0.0f && PlayerController->CurrentSideValue == 0.0f && m_Velocity.SizeSquared() < SMALL_NUMBER) {
		PlayerRef->StateManager->SwitchStateByKey("Grounded");
		return;
	}

	if (checkBelowPlayer.bBlockingHit && SlopeCheck(checkBelowPlayer.ImpactNormal)) {
		PlayerRef->StateManager->SwitchStateByKey("Sliding");
		return;
	} 

	/*if (checkBelowPlayer.bBlockingHit && !SlopeCheck(checkBelowPlayer.ImpactNormal)) {
		MovePlayerUpSlope(checkBelowPlayer, DeltaTime);
	}
	else {*/
		MovePlayer(checkBelowPlayer, DeltaTime);
	//}
	
}

void UWalkingState::OnExitState()
{
	Super::OnExitState();
	
	UE_LOG(LogTemp, Warning, TEXT("Leaving Walking state, velocity: %s"), *PlayerRef->PlayerMoveComponent->Velocity.ToString());
}


void UWalkingState::MovePlayer(FHitResult& hitResult, float deltaTime)
{
	/*FVector force = FVector::ZeroVector;
	force += PlayerRef->GetActorForwardVector() * WalkForce * PlayerController->CurrentFrontBackValue;
	force += PlayerRef->GetActorRightVector() * WalkForce * PlayerController->CurrentSideValue;
	force += GetAirResistance();

	FVector acceleration = force / 50;
	m_Velocity = acceleration * deltaTime;

	PlayerRef->PlayerMoveComponent->Velocity = m_Velocity;
	FHitResult hitResult;
	PlayerRef->AddActorWorldOffset(m_Velocity, true, &hitResult);

	if (hitResult.bBlockingHit && SlopeCheck(checkBelowRes.ImpactNormal)) {
		PlayerRef->PlayerMoveComponent->Velocity = FVector::ZeroVector;
		UE_LOG(LogTemp, Warning, TEXT("Hit obstacle while walking"));
	}
	else {
		MovePlayerUpSlope(checkBelowRes, deltaTime);
	}*/

	// get slide direction
	FVector playerUpVec = PlayerRef->GetActorUpVector();
	FVector slideDirection = playerUpVec - (FVector::DotProduct(playerUpVec, hitResult.ImpactNormal) / (hitResult.ImpactNormal.Size() * hitResult.ImpactNormal.Size())) * hitResult.ImpactNormal; // vector projection onto plane
	slideDirection.Normalize();

	// Calculate upward force to counteract gravity on slopes
	FVector upwardForce = FVector::ZeroVector;
	if (!SlopeCheck(hitResult.ImpactNormal)) // Walkable slope
	{
		UE_LOG(LogTemp, Warning, TEXT("It is a slope move up"));
		upwardForce = slideDirection * 600.0f; // Counteract sliding downward
	}

	FVector sideMovement = FVector::CrossProduct(hitResult.ImpactNormal, slideDirection).GetSafeNormal() * PlayerController->CurrentSideValue * 800.0f;

	FVector slopeForwardVec = FVector::CrossProduct(sideMovement, hitResult.ImpactNormal).GetSafeNormal() * PlayerController->CurrentFrontBackValue * 800.0f;

	// Combine all forces
	
	FVector totalMovement = slideDirection + slopeForwardVec + sideMovement;

	totalMovement *= deltaTime;
	PlayerRef->PlayerMoveComponent->Velocity = totalMovement;

	// Apply the calculated movement
	FHitResult hit;
	PlayerRef->AddActorWorldOffset(totalMovement, true, &hit);

	//if (hit.bBlockingHit && SlopeCheck(hit.ImpactNormal))
	//{
		//PlayerRef->PlayerMoveComponent->Velocity = FVector::ZeroVector;
		//UE_LOG(LogTemp, Warning, TEXT("Hit obstacle while walking"));
	//}
}

void UWalkingState::MovePlayerUpSlope(FHitResult& hitresult, float deltaTime)
{
	// get slide direction
	FVector playerUpVec = PlayerRef->GetActorUpVector();
	FVector slideDirection = playerUpVec - (FVector::DotProduct(playerUpVec, hitresult.ImpactNormal) / (hitresult.ImpactNormal.Size() * hitresult.ImpactNormal.Size())) * hitresult.ImpactNormal; // vector projection onto plane
	slideDirection.Normalize();

	FVector upwardForce = slideDirection * 400.0f;

	FVector fowardMovement = PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 800.0f;
	FVector sideMovement = PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 800.0f;
	FVector totalMovement = upwardForce + fowardMovement + sideMovement;

	totalMovement *= deltaTime;

	PlayerRef->PlayerMoveComponent->Velocity = totalMovement;
	FHitResult hitResult;
	PlayerRef->AddActorWorldOffset(totalMovement, true, &hitResult);

	if (hitResult.bBlockingHit && SlopeCheck(hitresult.ImpactNormal)) {
		PlayerRef->PlayerMoveComponent->Velocity = FVector::ZeroVector;
		UE_LOG(LogTemp, Warning, TEXT("Hit obstacle while walking"));
	}
	
}

FVector UWalkingState::GetAirResistance()
{
	return -m_Velocity.GetSafeNormal() * m_Velocity.SizeSquared() * DragCoefficient;
}


