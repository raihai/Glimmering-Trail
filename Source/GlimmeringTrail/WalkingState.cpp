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

	if (testFunc(checkBelowPlayer)) { // ledge check
		FallingFromLedge = true;
		PlayerRef->StateManager->SwitchStateByKey("Air");
		return;
	}

	if (PlayerController->CurrentFrontBackValue == 0.0f && PlayerController->CurrentSideValue == 0.0f && m_Velocity.SizeSquared() < SMALL_NUMBER) {
		PlayerRef->StateManager->SwitchStateByKey("Grounded");
		return;
	}

	if (checkBelowPlayer.bBlockingHit && SlopeCheck(checkBelowPlayer.ImpactNormal) ) {
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

bool UWalkingState::testFunc(FHitResult& hitResult)
{
	
		FVector belowPlayerLocation = PlayerRef->GetActorLocation() - FVector(0, 0, 80.0f); // Bottom of capsule
		FVector offsets[] = {
			FVector(50, 0, 0),
			FVector(-50, 0, 0),
			FVector(0, 50, 0),
			FVector(0, -50, 0)
		};

		FVector fallOffsets[] = {
			FVector(30, 0, 0),
			FVector(-30, 0, 0),
			FVector(0, 30, 0),
			FVector(0, -30, 0)
		};

		bool bStandHits[4] = { false, false, false, false };
		bool bFallHits[4] = { false, false, false, false };

		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(PlayerRef);

		int missedStandRays = 0;
		int missedFallRays = 0;

		for (int32 i = 0; i < 4; ++i) {
			FVector standStart = belowPlayerLocation + offsets[i];
			FVector standEnd = standStart - FVector(0, 0, 100.0f);

			FVector fallStart = belowPlayerLocation + fallOffsets[i];
			FVector fallEnd = fallStart - FVector(0, 0, 100.0f);

			bStandHits[i] = GetWorld()->LineTraceSingleByChannel(hitResult, standStart, standEnd, ECC_Visibility, CollisionParams); // stand
			DrawDebugLine(GetWorld(), standStart, standEnd, bStandHits[i] ? FColor::Green : FColor::Purple, false, 10.0f, 0, 1.0f);

			bFallHits[i] = GetWorld()->LineTraceSingleByChannel(hitResult, fallStart, fallEnd, ECC_Visibility, CollisionParams); // fall 
			DrawDebugLine(GetWorld(), fallStart, fallEnd, bFallHits[i] ? FColor::Red : FColor::Blue, false, 10.0f, 0, 1.0f);

			if (!bStandHits[i]) missedStandRays += 1;
			if (!bFallHits[i]) missedFallRays += 1;
		}


		
		if ((missedStandRays >= 3) && (missedFallRays == 4) ) {

			for (int i = 0; i < 4; i++) {
				if (bStandHits[i] == false && bFallHits[i] == false) {
					return true;
				}
			}
		}
		return false;

}


