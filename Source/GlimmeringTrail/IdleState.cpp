// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleState.h"
#include "PlayerMovementComponent.h"

/////////////////// properly handle transtion from idle to walking
//void UIdleState::HandleJump()
//{
//	// this works fine for now
//	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Purple, "Jumping from idle state");
//	float g = (2 * 300) / (0.44 * 0.44);
//	float jumpHeight = 300.0f;
//	float jumpVelocity = sqrt(2 * g * jumpHeight);
//	
//	PlayerRef->PlayerMoveComponent->Velocity.Z = jumpVelocity;
//
//	PlayerRef->StateManager->SwitchStateByKey("Air");
//}

void UIdleState::OnEnterState(AActor* OwnerRef)
{
	Super::OnEnterState(OwnerRef);
	/*m_FValue = 0;
	m_SValue = 0;*/
	//PlayerRef->PlayerMoveComponent->Velocity = FVector::ZeroVector;
	//UE_LOG(LogTemp, Warning, TEXT("Entering IdleState: Resetting FrontBackValue and SideValue to zero."));
	//UE_LOG(LogTemp, Warning, TEXT("Entering Idle State: FrontValue %f"), FrontBackValue);
	//UE_LOG(LogTemp, Warning, TEXT("Entering Idle State: SideValue %f"), SideValue);
}

//void UIdleState::HandleForwardBackwardMovement(const FInputActionValue& Value)
//{
//	// this does work properly, the movementinput is not registered straight away
//
//	Super::HandleForwardBackwardMovement(Value);
//	m_FValue = Value.Get<float>();
//	// Apply force based on input and transition to Walking
//	//float input = Value.Get<float>();
//	////if (input != 0.0f)
//	////{
//
//	//	// Switch to Walking as soon as there is forward/backward input
//	//	PlayerRef->StateManager->SwitchStateByKey("Walking");
//	//}
//}

//void UIdleState::HandleSidewayMovement(const FInputActionValue& Value)
//{
//	Super::HandleSidewayMovement(Value);
//	m_SValue = Value.Get<float>();
//	// Apply force based on input and transition to Walking
//	//float input = Value.Get<float>();
//	////if (input != 0.0f)
//	////{
//	////
//
//	//	// Switch to Walking as soon as there is forward/backward input
//	//	PlayerRef->StateManager->SwitchStateByKey("Walking");
//	//}
//}

//void UIdleState::HandleStopForwardMovement()
//{
//	Super::HandleStopForwardMovement();
//	FrontBackValue = 0;
//}
//
//void UIdleState::HandleStopSideMovement()
//{
//	Super::HandleStopSideMovement();
//	SideValue = 0;
//}

void UIdleState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);
	/*if (!PlayerRef->IsGrounded()) {
		PlayerRef->StateManager->SwitchStateByKey("Air");
	}*/
	// Add Switch to Walking State
	if (PlayerController->CurrentFrontBackValue != 0 || PlayerController->CurrentSideValue != 0) {


		/*UE_LOG(LogTemp, Warning, TEXT("Idle State: FrontValue %f"), FrontBackValue);
		UE_LOG(LogTemp, Warning, TEXT("Idle State: SideValue %f"), SideValue);*/
		PlayerRef->StateManager->SwitchStateByKey("Walking");
	}

	//Add Switch to Running States;
}

void UIdleState::OnExitState()
{

	Super::OnExitState();
	/*m_FValue = 0;
	m_SValue = 0;*/
}


