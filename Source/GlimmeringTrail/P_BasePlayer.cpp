// Fill out your copyright notice in the Description page of Project Settings.


#include "P_BasePlayer.h"
#include "GameFramework/Controller.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"



// Sets default values
AP_BasePlayer::AP_BasePlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlayerMoveComponent = CreateDefaultSubobject<UPlayerMovementComponent>(TEXT("MoveComponent"));
	
	
	// Set size for collision capsule
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootComponent"));
	SetRootComponent(CapsuleComponent);

	// Adjust the collision capsule properties as needed
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));

}

// Called when the game starts or when spawned
void AP_BasePlayer::BeginPlay()
{
	Super::BeginPlay();

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
void AP_BasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AP_BasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AP_BasePlayer::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AP_BasePlayer::StopJump);

		//Moving
		EnhancedInputComponent->BindAction(IA_ForwardBackwardMove, ETriggerEvent::Triggered, this, &AP_BasePlayer::MoveForwardBackward);
		EnhancedInputComponent->BindAction(IA_LeftRightMove, ETriggerEvent::Triggered, this, &AP_BasePlayer::MoveLeftRight);



	}
}

void AP_BasePlayer::MoveForwardBackward(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Moving Forward"));
	if (PlayerMoveComponent == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("No moveCmoponent found "));
		return;
	}

	
	PlayerMoveComponent->SetMoveForwardValue(Value.Get<float>());
	
}

void AP_BasePlayer::MoveLeftRight(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Moving Sideway"));
	if (PlayerMoveComponent == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("No moveComponent found "));
		return;
	}
	PlayerMoveComponent->SetMoveSidewayValue(Value.Get<float>());

}

void AP_BasePlayer::Jump(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Jump"));
}

void AP_BasePlayer::StopJump(const FInputActionValue& Valuee)
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Jump"));
}

