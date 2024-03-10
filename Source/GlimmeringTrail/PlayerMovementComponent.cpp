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
	
	if (move) {
		SimulateMovement(DeltaTime);
	}

}

void UPlayerMovementComponent::SimulateMovement(float DeltaTime)
{
	// this is for character going forward
	FVector force = GetOwner()->GetActorForwardVector() * MaxDriveForce *  MoveForwardBackwardValue;
	force += GetAirResistance();

	FVector force2 = GetOwner()->GetActorRightVector() * MaxDriveForce * MoveSideVal;

	force += force2;
	
	UE_LOG(LogTemp, Warning, TEXT(" MFBV : %f AND MSV: %f"), MoveForwardBackwardValue, MoveSideVal);

	FVector acceleration = force / Mass;

	// ensure framerate independance
	Velocity = Velocity + acceleration * DeltaTime;

	//ApplyRotation(DeltaTime, MoveForwardBackwardValue);
	UpdateLocationFromVelocity(DeltaTime);
}


FVector UPlayerMovementComponent::GetAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}


void UPlayerMovementComponent::ApplyRotation(float DeltaTime, float MoveSideValue)
{
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

