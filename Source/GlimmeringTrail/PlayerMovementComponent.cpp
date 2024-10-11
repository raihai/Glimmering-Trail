// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovementComponent.h"

// Sets default values for this component's properties
UPlayerMovementComponent::UPlayerMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UPlayerMovementComponent::SetMoveForwardValue(float AxisValue)
{
	MoveForwardBackwardValue = AxisValue; // Reset quick stop flag if input is active
	if (AxisValue != 0.0f){
			bQuickStop = false;
	}
}

void UPlayerMovementComponent::SetMoveSidewayValue(float AxisValue)
{
	MoveSideVal = AxisValue; 
	if (AxisValue != 0.0f) {
		bQuickStop = false;
	}
}

void UPlayerMovementComponent::StopMovement()
{
	bQuickStop = true;
	MoveForwardBackwardValue = 0.0f;
	MoveSideVal = 0.0f;
}

/*oid UPlayerMovementComponent::HandleMovement(EPlayerGameState CurrentState, float DeltaTime)
{
	switch (CurrentState) {
	case EPlayerGameState::Idle:
		StopMovement();
		SimulateMovement(DeltaTime, 0);
		UE_LOG(LogTemp, Warning, TEXT("Idle state "));
		break;
	case  EPlayerGameState::Walking:
		HandleWalking(DeltaTime);
		UE_LOG(LogTemp, Warning, TEXT("walking state "));
		break;
	case  EPlayerGameState::Running:
		HandleRunning(DeltaTime);
		UE_LOG(LogTemp, Warning, TEXT("Running state "));
		break;
	case  EPlayerGameState::Jumping:
		HandleJumping(DeltaTime);
		UE_LOG(LogTemp, Warning, TEXT("Jumping State"));
		break;
	case EPlayerGameState::Falling:
		HandleFalling(DeltaTime);
		UE_LOG(LogTemp, Warning, TEXT("Falling State"));
		break;
	case  EPlayerGameState::WallJump:
		break;
	case  EPlayerGameState::WallSlide:
		break;
	case  EPlayerGameState::Dash:
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("No state found"));
	}*/
//}

void UPlayerMovementComponent::SimulateMovement(float DeltaTime, float MoveForce )
{
	FVector force = GetOwner()->GetActorForwardVector() * MoveForce *  MoveForwardBackwardValue;
	force += GetOwner()->GetActorRightVector() * MoveForce * MoveSideVal; 
	force += GetAirResistance();

	FVector acceleration = force / Mass;
	Velocity += acceleration * DeltaTime;

	if (bQuickStop){
		Velocity *= 0.8f; 
	}

	if (Velocity.SizeSquared() < KINDA_SMALL_NUMBER){
		Velocity = FVector::ZeroVector;
	}

	ApplyRotation(DeltaTime, MoveForwardBackwardValue);
	UpdateLocationFromVelocity(DeltaTime);
}

void UPlayerMovementComponent::HandleWalking(float DeltaTime)
{
	SimulateMovement(DeltaTime, 600);
}

void UPlayerMovementComponent::HandleRunning(float DeltaTime)
{
	SimulateMovement(DeltaTime, 2000);
}

void UPlayerMovementComponent::HandleJumping(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Jumping right now"));


	//velocity = velocity + gravity * dt
	//position = position + velocity * dt

	//--what is the gravity that would allow jumping to a given height ?
	//	g = (2 * jumpHeight) / (timeToApex ^ 2)

	//	--what is the initial jump velocity ?
	//	initJumpVelocity = math.sqrt(2 * g * jumpHeight)

	//	--how long does it take to reach the maximum height of a jump ?
	//	--note : if "initJumpVelocity" is not a multiple of "g" the maximum height is reached between frames
	//	timeToApex = initJumpVelocity / g
	

	//float timeToApex = testJumpVelocity / Gravity;
	float g = FMath::Abs(GetWorld()->GetGravityZ());
	
	testJumpVelocity = sqrt(2 * g * JumpHeight);
	FVector UpwardForce = GetOwner()->GetActorUpVector() * testJumpVelocity;

	FVector acc = UpwardForce / Mass;
	Velocity += acc + DeltaTime;
	
	FVector Translation = Velocity * DeltaTime;

	FHitResult hitResult;
	GetOwner()->AddActorWorldOffset(Translation, true, &hitResult);

	if (hitResult.IsValidBlockingHit()) {
		Velocity = FVector::ZeroVector;
	}
}

void UPlayerMovementComponent::HandleFalling(float DeltaTime)
{
	// need to push player down
	UE_LOG(LogTemp, Warning, TEXT("Falling down now"));

	Velocity += FVector(0, 0, -200.0f) * DeltaTime;
	UpdateLocationFromVelocity(DeltaTime);
}

FVector UPlayerMovementComponent::GetAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

void UPlayerMovementComponent::ApplyRotation(float DeltaTime, float MoveSideValue)
{
	// apply torque to the player 
	return;
}

void UPlayerMovementComponent::UpdateLocationFromVelocity(float DeltaTime)
{
	FVector Translation = Velocity * 100 * DeltaTime; 
	
	FHitResult hitResult;
	GetOwner()->AddActorWorldOffset(Translation, true, &hitResult);

	if (hitResult.IsValidBlockingHit()) {
		Velocity = FVector::ZeroVector;
	}
}

