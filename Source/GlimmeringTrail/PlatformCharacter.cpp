// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformCharacter.h"
#include "PlayerMovementComponent.h"
#include "GameFramework/Controller.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
APlatformCharacter::APlatformCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MoveComponent = CreateDefaultSubobject<UPlayerMovementComponent>(TEXT("MoveComponent"));

}

// Called when the game starts or when spawned
void APlatformCharacter::BeginPlay()

{

	Super::BeginPlay();
	//Add Input Mapping Context

	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerInputMap, 0);
		}
	}
	
}

// Called every frame
void APlatformCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlatformCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
	
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlatformCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlatformCharacter::StopJump);

		//Moving
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &APlatformCharacter::MoveForward);
		EnhancedInputComponent->BindAction(MoveSidewayAction, ETriggerEvent::Triggered, this, &APlatformCharacter::MoveSideway);

	}

}

void APlatformCharacter::MoveForward(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Moving Forward"));
	if (MoveComponent == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("No moveCmoponent found "));
		return;
	}
	MoveComponent->SetMoveForwardValue(Value.Get<float>());
	UE_LOG(LogTemp, Warning, TEXT("Moving Forward"));
}

void APlatformCharacter::MoveSideway(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Moving Sideways"));
	if (MoveComponent == nullptr)return;
	MoveComponent->SetMoveSidewayValue(Value.Get<float>());
	UE_LOG(LogTemp, Warning, TEXT("Moving Sideways"));
}

void APlatformCharacter::Jump(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Jump"));
}

void APlatformCharacter::StopJump(const FInputActionValue& Valuee)
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Jump"));
}

