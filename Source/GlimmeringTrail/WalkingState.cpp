// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingState.h"

void UWalkingState::OnEnterState(AActor* OwnerRef)
{
	Super::OnEnterState(OwnerRef);
	UE_LOG(LogTemp, Warning, TEXT("Entering Walking STATE"));
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

	MovePlayer(checkBelowPlayer, DeltaTime);

	
}

void UWalkingState::OnExitState()
{
	Super::OnExitState();
	m_Velocity = FVector::ZeroVector;
	UE_LOG(LogTemp, Warning, TEXT("Leaving Walking state, velocity: %s"), *PlayerRef->PlayerMoveComponent->Velocity.ToString());
}


void UWalkingState::MovePlayer(FHitResult& hitResult, float deltaTime)
{
	FVector force = FVector::ZeroVector;
	FVector impactNormal = hitResult.ImpactNormal.GetSafeNormal();

	if (FMath::Abs(PlayerController->CurrentFrontBackValue) == FMath::Abs(PlayerController->CurrentSideValue)) {
		force += FVector::VectorPlaneProject(PlayerRef->GetActorForwardVector(), impactNormal).GetSafeNormal() * halfWalkForce * PlayerController->CurrentFrontBackValue;
		force += FVector::VectorPlaneProject(PlayerRef->GetActorRightVector(), impactNormal).GetSafeNormal() * halfWalkForce * PlayerController->CurrentSideValue;
	}
	else {
		force += FVector::VectorPlaneProject(PlayerRef->GetActorForwardVector(), impactNormal).GetSafeNormal() * WalkForce * PlayerController->CurrentFrontBackValue;
		force += FVector::VectorPlaneProject(PlayerRef->GetActorRightVector(), impactNormal).GetSafeNormal() * WalkForce * PlayerController->CurrentSideValue;
	}
	
	//float inputMagnitude = FMath::Sqrt(FMath::Square(PlayerController->CurrentFrontBackValue) + FMath::Square(PlayerController->CurrentSideValue));
	//if (inputMagnitude > 1.0f) {
	//	PlayerController->CurrentFrontBackValue /= inputMagnitude;
	//	PlayerController->CurrentSideValue /= inputMagnitude;
	//}


	force += GetAirResistance();
	
	FVector acceleration = force / 50;
	m_Velocity = acceleration * deltaTime;
	PlayerRef->PlayerMoveComponent->Velocity = m_Velocity;

	FHitResult moveResult;
	PlayerRef->AddActorWorldOffset(m_Velocity, true, &moveResult);

	if (hitResult.bBlockingHit && SlopeCheck(hitResult.ImpactNormal)) {
		PlayerRef->PlayerMoveComponent->Velocity = FVector::ZeroVector;
		UE_LOG(LogTemp, Warning, TEXT("Hit obstacle while walking"));
	}
	
}

//void UWalkingState::MovePlayerUpSlope(FHitResult& hitresult, float deltaTime)
//{
//	// get slide direction
//	FVector playerUpVec = PlayerRef->GetActorUpVector();
//	FVector slideDirection = playerUpVec - (FVector::DotProduct(playerUpVec, hitresult.ImpactNormal) / (hitresult.ImpactNormal.Size() * hitresult.ImpactNormal.Size())) * hitresult.ImpactNormal; // vector projection onto plane
//	slideDirection.Normalize();
//
//	FVector upwardForce = slideDirection * 400.0f;
//
//	FVector fowardMovement = PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 800.0f;
//	FVector sideMovement = PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 800.0f;
//	FVector totalMovement = upwardForce + fowardMovement + sideMovement;
//
//	totalMovement *= deltaTime;
//
//	PlayerRef->PlayerMoveComponent->Velocity = totalMovement;
//	FHitResult hitResult;
//	PlayerRef->AddActorWorldOffset(totalMovement, true, &hitResult);
//
//	if (hitResult.bBlockingHit && SlopeCheck(hitresult.ImpactNormal)) {
//		PlayerRef->PlayerMoveComponent->Velocity = FVector::ZeroVector;
//		UE_LOG(LogTemp, Warning, TEXT("Hit obstacle while walking"));
//	}
//	
//}

FVector UWalkingState::GetAirResistance()
{
	return -m_Velocity.GetSafeNormal() * m_Velocity.SizeSquared() * DragCoefficient;
}


