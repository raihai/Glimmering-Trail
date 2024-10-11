// Fill out your copyright notice in the Description page of Project Settings.


#include "P_BasePlayer.h"
#include "PlayerMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AP_BasePlayer::AP_BasePlayer()
{

	StateManager = CreateDefaultSubobject<UPlayerStateComponent>(TEXT("State Manager"));

	PrimaryActorTick.bCanEverTick = true;
	PlayerMoveComponent = CreateDefaultSubobject<UPlayerMovementComponent>(TEXT("MoveComponent"));
	
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RootComponent"));
	SetRootComponent(CapsuleComponent);

	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));

	//GRavity
	//RootComponent->Gravity

	/*CurrMainState = EPlayerMainState::Grounded;
	CurrGroundState = EPlayerGroundedSubState::Idle;
	CurrAirborneState = EPlayerAirborneSubState::Falling;*/
}

// Called when the game starts or when spawned
void AP_BasePlayer::BeginPlay()
{
	Super::BeginPlay();

	StateManager->InitStateManager();

	/*APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerInputMap, 0);
		}
	}*/
	
}

// Called every frame
void AP_BasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (PlayerMoveComponent) {

	//	if (bJumpPressed) {
	//		JumpKeyHoldTime += DeltaTime;
	//	}

	//	//PlayerMoveComponent->HandleMovement(CurrentState, DeltaTime);

	//}
}

// Called to bind functionality to input
void AP_BasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		//EnhancedInputComponent->BindAction(IA_JumpUp, ETriggerEvent::Triggered, this, &AP_BasePlayer::Jump);
	//	EnhancedInputComponent->BindAction(IA_JumpUp, ETriggerEvent::Completed, this, &AP_BasePlayer::StopJump);

		//Moving
		//EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &AP_BasePlayer::MoveForwardBackward);
		//EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Completed, this, &AP_BasePlayer::StopMoveForwardBackward);
		//EnhancedInputComponent->BindAction(IA_MoveSideway, ETriggerEvent::Triggered, this, &AP_BasePlayer::MoveLeftRight);
		//EnhancedInputComponent->BindAction(IA_MoveSideway, ETriggerEvent::Completed, this, &AP_BasePlayer::StopMoveLeftRight);

		////movement modifiers
		//EnhancedInputComponent->BindActionValueLambda(IA_Run, ETriggerEvent::Triggered, [this](const FInputActionValue& InputActionValue){
		//	bIsRunning = true; });
		//EnhancedInputComponent->BindActionValueLambda(IA_Run, ETriggerEvent::Completed, [this](const FInputActionValue& InputActionValue) {
		//	bIsRunning = false; });
	}
}

//void AP_BasePlayer::ChangeState(EPlayerGameState NewState)
//{
//	if (CurrentState != NewState){
//		CurrentState = NewState;
//	}
//}


void AP_BasePlayer::MoveForwardBackward(const FInputActionValue& Value)
{
	/*if (PlayerMoveComponent == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("No MoveComponent found "));
		return;
	}
	PlayerMoveComponent->SetMoveForwardValue(Value.Get<float>());

	if (bIsRunning && IsGrounded()) { CurrGroundState = EPlayerGroundedSubState::Running; }
	else {
		CurrGroundState = EPlayerGroundedSubState::Walking;
		CurrAirborneState = EPlayerAirborneSubState::Falling;
	};*/
}

void AP_BasePlayer::StopMoveForwardBackward(const FInputActionValue& Value)
{/*
	CurrGroundState = EPlayerGroundedSubState::Idle;
	CurrAirborneState = EPlayerAirborneSubState::Falling;*/
}

void AP_BasePlayer::MoveLeftRight(const FInputActionValue& Value)
{/*
	if (PlayerMoveComponent == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("No MoveComponent found "));
		return;
	}

	PlayerMoveComponent->SetMoveSidewayValue(Value.Get<float>());
	if (bIsRunning && IsGrounded()) { CurrGroundState = EPlayerGroundedSubState::Running;}
	else {	
		CurrGroundState = EPlayerGroundedSubState::Walking; 
		CurrAirborneState = EPlayerAirborneSubState::Falling;
	};*/
}

void AP_BasePlayer::StopMoveLeftRight(const FInputActionValue& Value)
{
	/*CurrGroundState = EPlayerGroundedSubState::Idle;
	CurrAirborneState = EPlayerAirborneSubState::Falling;*/
}

void AP_BasePlayer::Jump(const FInputActionValue& Value)
{
	bJumpPressed = true;
	if (IsGrounded() && JumpKeyHoldTime <= 1.0f) {
		//CurrMainState = EPlayerMainState::Airborne;
	} else{
		bJumpPressed = false;
		JumpKeyHoldTime = 0.0f;
		//CurrentState = EPlayerGameState::Falling;
	}
}

void AP_BasePlayer::StopJump(const FInputActionValue& Valuee)
{
	//if (IsGrounded()) {
	//	CurrMainState = EPlayerMainState::Grounded;
	//}
	//else {
	//	//CurrentState = EPlayerGameState::Falling;
	//}
}

bool AP_BasePlayer::IsGrounded()
{
	FHitResult hitResult;
	FVector startLocation = GetActorLocation();
	FVector endLocation = startLocation - FVector(0, 0, 110.0f);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // Ignore the player itself in the raycast

	bool bHit = GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, ECC_Visibility, CollisionParams);

	DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, 10.0f, 0, 5.0f);
	return bHit;
}

