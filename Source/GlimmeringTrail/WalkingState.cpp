// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingState.h"

void UWalkingState::OnEnterState(AActor* OwnerRef)
{
	Super::OnEnterState(OwnerRef);
}

void UWalkingState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);

	//************ ensure that the gravity is always pusing down 
	if (PlayerController->CurrentFrontBackValue == 0.0f && PlayerController->CurrentSideValue == 0.0f && m_Velocity.SizeSquared() < SMALL_NUMBER) {
		
		PlayerRef->StateManager->SwitchStateByKey("Grounded");
		return;
	}

	FHitResult checkBelowPlayer;
	if (IsGrounded(checkBelowPlayer)) {

		PlayerRef->StateManager->SwitchStateByKey("Airborne");
		return;
	}
	
	if (SlopeCheck(checkBelowPlayer.ImpactNormal)) {
		UE_LOG(LogTemp, Warning, TEXT("There is a slope"));
		PlayerRef->StateManager->SwitchStateByKey("Sliding");
		return;
	}

	FVector force = FVector::ZeroVector;
	force += PlayerRef->GetActorForwardVector() * WalkForce * PlayerController->CurrentFrontBackValue;
	force += PlayerRef->GetActorRightVector() * WalkForce * PlayerController->CurrentSideValue;
	force += GetAirResistance();

	FVector acceleration = force / 50;
	m_Velocity = acceleration * DeltaTime;


	PlayerRef->PlayerMoveComponent->Velocity = m_Velocity;
	FHitResult hitResult;

	PlayerRef->AddActorWorldOffset(m_Velocity, true, &hitResult);
}

void UWalkingState::OnExitState()
{
	Super::OnExitState();
}


FVector UWalkingState::GetAirResistance()
{
	return -m_Velocity.GetSafeNormal() * m_Velocity.SizeSquared() * DragCoefficient;
}


