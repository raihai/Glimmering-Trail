// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void APlatformPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{

}

void APlatformPlayerController::PressJumpButton()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "I pressed the jump key");
	if (JumpDelegate.IsBound()) {
		JumpDelegate.Broadcast(); // executes all the bound func // to the state
	}
}

void APlatformPlayerController::PressForwardBackwardButton(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();

	if (InputValue > 0.0f) // Forward button pressed
	{
		bIsForwardPressed = true;
	}
	else if (InputValue < 0.0f) // Backward button pressed
	{
		bIsBackwardPressed = true;
	}

	UpdateCurrentFrontBackValue();
}


void APlatformPlayerController::PressSidewaysButtonButton(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();

	if (InputValue > 0.0f) // Forward button pressed
	{
		bIsMoveRightPressed = true;
	}
	else if (InputValue < 0.0f) // Backward button pressed
	{
		bIsMoveLeftPressed = true;
	}

	UpdateCurrentSideValue();

}

void APlatformPlayerController::PressRunButton()
{

	if (RunDelegate.IsBound()) {
		RunDelegate.Broadcast();
	}
}

void APlatformPlayerController::ReleaseForwardButton()
{
	bIsForwardPressed = false;
	UpdateCurrentFrontBackValue();
}

void APlatformPlayerController::ReleaseBackwardButton()
{
	bIsBackwardPressed = false;
	UpdateCurrentFrontBackValue();
}

void APlatformPlayerController::ReleaseSidewayRight()
{
	bIsMoveRightPressed = false;
	UpdateCurrentSideValue();
}

void APlatformPlayerController::ReleaseSideWayLeft()
{
	bIsMoveLeftPressed = false;
	UpdateCurrentSideValue();
}

float APlatformPlayerController::GetCurrentFrontBackValue() const
{
	return CurrentFrontBackValue;
}

float APlatformPlayerController::GetCurrentSideValue() const
{
	return CurrentSideValue;
}

void APlatformPlayerController::UpdateCurrentFrontBackValue()
{
	if (bIsForwardPressed && bIsBackwardPressed)
	{
		CurrentFrontBackValue = 0.0f; // Cancel movement if both are pressed
	}
	else if (bIsForwardPressed)
	{
		CurrentFrontBackValue = 1.0f; // Move forward
	}
	else if (bIsBackwardPressed)
	{
		CurrentFrontBackValue = -1.0f; // Move backward
	}
	else
	{
		CurrentFrontBackValue = 0.0f; // Stop movement if neither is pressed
	}
}

void APlatformPlayerController::UpdateCurrentSideValue()
{
	if (bIsMoveRightPressed && bIsMoveLeftPressed)
	{
		CurrentSideValue = 0.0f; // Cancel movement if both are pressed
	}
	else if (bIsMoveRightPressed)
	{
		CurrentSideValue = 1.0f; // Move forward
	}
	else if (bIsMoveLeftPressed)
	{
		CurrentSideValue = -1.0f; // Move backward
	}
	else
	{
		CurrentSideValue = 0.0f; // Stop movement if neither is presse
	}
	
}

void APlatformPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		//jumping
		EnhancedInputComponent->BindAction(IA_JumpUp, ETriggerEvent::Triggered, this, &APlatformPlayerController::PressJumpButton);
		EnhancedInputComponent->BindAction(IA_JumpUp, ETriggerEvent::Completed, this, &APlatformPlayerController::PressJumpButton);

		//Moving Forward
		EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &APlatformPlayerController::PressForwardBackwardButton);
		EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Completed, this, &APlatformPlayerController::ReleaseForwardButton);
		//Moving Back
		EnhancedInputComponent->BindAction(IA_MoveBackward, ETriggerEvent::Triggered, this, &APlatformPlayerController::PressForwardBackwardButton);
		EnhancedInputComponent->BindAction(IA_MoveBackward, ETriggerEvent::Completed, this, &APlatformPlayerController::ReleaseBackwardButton);

		//Moving Right
		EnhancedInputComponent->BindAction(IA_MoveSideway, ETriggerEvent::Triggered, this, &APlatformPlayerController::PressSidewaysButtonButton);
		EnhancedInputComponent->BindAction(IA_MoveSideway, ETriggerEvent::Completed, this, &APlatformPlayerController::ReleaseSidewayRight);
		
		//Moving Left
		EnhancedInputComponent->BindAction(IA_MoveSideLeft, ETriggerEvent::Triggered, this, &APlatformPlayerController::PressSidewaysButtonButton);
		EnhancedInputComponent->BindAction(IA_MoveSideLeft, ETriggerEvent::Completed, this, &APlatformPlayerController::ReleaseSideWayLeft);

		//movement modifiers
		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Triggered, this, &APlatformPlayerController::PressRunButton);
		//EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Completed, &APlatformPlayerController::ReleaseMovementButton);
	}
}

void APlatformPlayerController::BeginPlay()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (Subsystem)
	{
		Subsystem->AddMappingContext(PlayerInputMap, 0);
	}
}
FJUMPSIGNNATURE* APlatformPlayerController::GetJumpDelegate()
{
	return &JumpDelegate;
}

FFRONTBACKSIGNNATURE* APlatformPlayerController::GetFrontBackDelegate()
{
	return &ForwardBackwardDelegate;
}

FSIDEWAYSIGNNATURE* APlatformPlayerController::GetSidewayDelegate()
{
	return &SidewayDelegate;
}

FRUNSIGNNATURE* APlatformPlayerController::GetRunDelegate()
{
	return &RunDelegate;
}

FSTOPFORWARDSIGNAUTURE* APlatformPlayerController::GetMoveForwardStopDelegate()
{
	return &StopForwardMovementDelegate;
}

FSTOPSIDESIGNAUTURE* APlatformPlayerController::GetMoveSideStopDelegate()
{
	return &StopSideMovementDelegate;
}
