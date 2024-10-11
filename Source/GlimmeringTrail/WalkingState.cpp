// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingState.h"

void UWalkingState::OnEnterState(AActor* OwnerRef)
{
	Super::OnEnterState(OwnerRef);


}

void UWalkingState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);

	FVector force = PlayerRef->GetActorForwardVector() * WalkForce * FrontBackValue ;
	force += PlayerRef->GetActorRightVector() * WalkForce * SideValue;
	force += GetAirResistance();

	/*FVector CurrVec = PlayerRef->PlayerMoveComponent->Velocity;
	CurrVec *= PlayerRef->GetActorForwardVector() * WalkForce* FrontBackValue;
	CurrVec = PlayerRef->GetActorRightVector() * WalkForce * SideValue;*/

	FVector acceleration = force / 50;

	/*if (bQuickStop) {
		acceleration *= 0.8f;
	}*/


	FVector Translation = acceleration * DeltaTime * 100;
	FHitResult hitResult;
	PlayerRef->AddActorWorldOffset(Translation, true, &hitResult);

	if (PlayerRef->PlayerMoveComponent->Velocity.SizeSquared() < KINDA_SMALL_NUMBER) {
		
		PlayerRef->StateManager->SwitchStateByKey("Idle");
	}
}

void UWalkingState::HandleForwardBackwardMovement(const FInputActionValue& Value)
{
	Super::HandleForwardBackwardMovement(Value);
	FrontBackValue = Value.Get<float>();
}

void UWalkingState::HandleSidewayMovment(const FInputActionValue& Value)
{
	Super::HandleSidewayMovment(Value);
	SideValue = Value.Get<float>();
}

void UWalkingState::HandleStopXYMovment()
{
	bQuickStop = true;
	FrontBackValue = 0;
	SideValue = 0;
}

void UWalkingState::HandleJump()
{
	
	float g = (2 * 300) / (0.44 * 0.44);
	float jumpHeight = 300.0f;

	float jumpVelocity = sqrt(2 * g * jumpHeight);

	FVector currVelocity = PlayerRef->PlayerMoveComponent->Velocity;
	currVelocity.Z = jumpVelocity;

	PlayerRef->PlayerMoveComponent->Velocity = currVelocity;

	PlayerRef->StateManager->SwitchStateByKey("Air");

}

FVector UWalkingState::GetAirResistance()
{
	return -m_Velocity.GetSafeNormal() * m_Velocity.SizeSquared() * DragCoefficient;

}

