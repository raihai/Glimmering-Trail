// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundedState.h"

void UGroundedState::HandleJump()
{
	Super::HandleJump();
}

void UGroundedState::OnEnterState(AActor* OwnerRef)
{
	Super::OnEnterState(OwnerRef);

	UE_LOG(LogTemp, Warning, TEXT("Entering Grounded State"));
}

void UGroundedState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);

	FHitResult checkBelowPlayer;

	if (IsGroundedRay(checkBelowPlayer)){
		ApplyFloatingEffect(checkBelowPlayer, DeltaTime);
	}

	if (!IsGroundedRay(checkBelowPlayer)) {
		PlayerRef->StateManager->SwitchStateByKey("Air");
	}
	
	if (SlopeCheck(checkBelowPlayer.ImpactNormal)) {
		UE_LOG(LogTemp, Warning, TEXT("There is a slope"));
		PlayerRef->StateManager->SwitchStateByKey("Sliding");
		return;
	}
	
	if (!FMath::IsNearlyZero(PlayerController->CurrentFrontBackValue) || !FMath::IsNearlyZero(PlayerController->CurrentSideValue)) {
		PlayerRef->StateManager->SwitchStateByKey("Walking");
		return;
	}
}

void UGroundedState::OnExitState()
{
	Super::OnExitState();
	UE_LOG(LogTemp, Warning, TEXT("lEAVING Grounded Velocity: %s"), *PlayerRef->PlayerMoveComponent->Velocity.ToString());
}
