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
	if (ForwardBackwardDelegate.IsBound()) {
		ForwardBackwardDelegate.Broadcast(Value);
	}
}

void APlatformPlayerController::PressSidewaysButtonButton(const FInputActionValue& Value)
{
	if (SidewayDelegate.IsBound()) {
		SidewayDelegate.Broadcast(Value);
	}
}

void APlatformPlayerController::PressRunButton()
{
	if (RunDelegate.IsBound()) {
		RunDelegate.Broadcast();
	}
}

void APlatformPlayerController::ReleaseMovementButton()
{
	if (StopDelegate.IsBound()) {
		StopDelegate.Broadcast();
	}

}


void APlatformPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		//jumping
		EnhancedInputComponent->BindAction(IA_JumpUp, ETriggerEvent::Triggered, this, &APlatformPlayerController::PressJumpButton);
		EnhancedInputComponent->BindAction(IA_JumpUp, ETriggerEvent::Completed, this, &APlatformPlayerController::PressJumpButton);

		//Moving
		EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &APlatformPlayerController::PressForwardBackwardButton);
		EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Completed, this, &APlatformPlayerController::ReleaseMovementButton);
		EnhancedInputComponent->BindAction(IA_MoveSideway, ETriggerEvent::Triggered, this, &APlatformPlayerController::PressSidewaysButtonButton);
		EnhancedInputComponent->BindAction(IA_MoveSideway, ETriggerEvent::Completed, this, &APlatformPlayerController::ReleaseMovementButton);

		////movement modifiers
		EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Triggered, this, &APlatformPlayerController::PressRunButton);
		//EnhancedInputComponent->BindAction(IA_Run, ETriggerEvent::Completed, &APlatformPlayerController::ReleaseMovementButton);

	}
}

void APlatformPlayerController::BeginPlay()
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (Subsystem)
	{
		// Add the input mapping context with priority 0
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

FSTOPSIGNAUTURE* APlatformPlayerController::GetMoveXYStopDelegate()
{
	return &StopDelegate;
}
