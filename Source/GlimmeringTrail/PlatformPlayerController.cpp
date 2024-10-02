// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void APlatformPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{

}

void APlatformPlayerController::PressJump()
{

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "I pressed the jump key");
	if (JumpDelegate.IsBound()) {
		JumpDelegate.Broadcast();
	}
}

void APlatformPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// bind actions
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent)) {
		EnhancedInputComponent->BindAction(IA_JumpUp, ETriggerEvent::Triggered, this, &APlatformPlayerController::PressJump);

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
