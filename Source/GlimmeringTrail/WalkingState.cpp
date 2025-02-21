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

	if (PlayerController->CurrentFrontBackValue == 0.0f && PlayerController->CurrentSideValue == 0.0f && m_Velocity.SizeSquared() < SMALL_NUMBER) {
		PlayerRef->StateManager->SwitchStateByKey("Grounded");
		return;
	}

	if (IsPlayerOnEdge(checkBelowPlayer)) { // this function triggers 
		UE_LOG(LogTemp, Warning, TEXT("Is one ledge, going to airborne state and the falling from ledge is true"));
		PlayerRef->StateManager->SwitchStateByKey("Air");
		return;
	}

	//if (!IsGroundedRay(checkBelowPlayer)) {
	//	PlayerRef->StateManager->SwitchStateByKey("Air");
	//	return;
	//}

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
	FVector impactNormal = hitResult.ImpactNormal;

	if (FMath::IsNearlyZero(PlayerController->CurrentFrontBackValue) && FMath::IsNearlyZero(PlayerController->CurrentSideValue))return;

	if (FMath::Abs(PlayerController->CurrentFrontBackValue) == FMath::Abs(PlayerController->CurrentSideValue)) {
		force += FVector::VectorPlaneProject(PlayerRef->GetActorForwardVector(), impactNormal) * halfWalkForce * PlayerController->CurrentFrontBackValue;
		force += FVector::VectorPlaneProject(PlayerRef->GetActorRightVector(), impactNormal) * halfWalkForce * PlayerController->CurrentSideValue;
	}
	else {
		force += FVector::VectorPlaneProject(PlayerRef->GetActorForwardVector(), impactNormal) * WalkForce* PlayerController->CurrentFrontBackValue;
		force += FVector::VectorPlaneProject(PlayerRef->GetActorRightVector(), impactNormal) * WalkForce * PlayerController->CurrentSideValue;
	}

	force += GetAirResistance();
	FVector acceleration = force / 40;
	m_Velocity = acceleration * deltaTime;
	PlayerRef->PlayerMoveComponent->Velocity = m_Velocity;

	ApplyFloatingEffect(hitResult, deltaTime);

	FHitResult moveResult;
	PlayerRef->AddActorWorldOffset(m_Velocity, true, &moveResult);

	if (hitResult.bBlockingHit && SlopeCheck(hitResult.ImpactNormal)) {
		PlayerRef->PlayerMoveComponent->Velocity = FVector::ZeroVector;
		UE_LOG(LogTemp, Warning, TEXT("Hit obstacle while walking"));
	}
	
}

FVector UWalkingState::GetAirResistance()
{
	return -m_Velocity.GetSafeNormal() * m_Velocity.SizeSquared() * DragCoefficient;
}

bool UWalkingState::IsPlayerOnEdge(FHitResult& hitResult)
{
		FVector belowPlayerLocation = PlayerRef->GetActorLocation() - FVector(0, 0, 90.0f); // Bottom of capsule
		FVector offsets[] = {
			FVector(50, 0, 0),
			FVector(-50, 0, 0),
			FVector(0, 50, 0),
			FVector(0, -50, 0)
		};

		FVector fallOffsets[] = {
			FVector(27, 0, 0),
			FVector(-27, 0, 0),
			FVector(0, 27, 0),
			FVector(0, -27, 0)
		};

		bool bStandHits[4] = { false, false, false, false };
		bool bFallHits[4] = { false, false, false, false };

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(PlayerRef);

		int missedStandRays = 0;
		int missedFallRays = 0;
		FVector edgeNormal = FVector::ZeroVector;

		for (int32 i = 0; i < 4; ++i) {
			FVector standStart = belowPlayerLocation + offsets[i];
			FVector standEnd = standStart - FVector(0, 0, 100.0f);

			FVector fallStart = belowPlayerLocation + fallOffsets[i];
			FVector fallEnd = fallStart - FVector(0, 0, 100.0f);

			bStandHits[i] = GetWorld()->LineTraceSingleByChannel(hitResult, standStart, standEnd, ECC_Visibility, CollisionParams); // stand
			DrawDebugLine(GetWorld(), standStart, standEnd, bStandHits[i] ? FColor::Blue : FColor::Purple, false, 10.0f, 0, 1.0f);

			bFallHits[i] = GetWorld()->LineTraceSingleByChannel(hitResult, fallStart, fallEnd, ECC_Visibility, CollisionParams); // fall 
			DrawDebugLine(GetWorld(), fallStart, fallEnd, bFallHits[i] ? FColor::Red : FColor::Purple, false, 10.0f, 0, 1.0f);

			if (!bStandHits[i]) missedStandRays += 1;
			if (!bFallHits[i]) missedFallRays += 1;
		}

		if ((missedStandRays >= 3) && (missedFallRays == 4) ) {
			return true;
		}
		return false;

}


