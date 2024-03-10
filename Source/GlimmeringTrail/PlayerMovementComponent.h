// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GLIMMERINGTRAIL_API UPlayerMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetMoveForwardValue(float AxisValue) { MoveForwardBackwardValue = AxisValue;}
	void SetMoveSidewayValue(float AxisValue) { MoveSideVal = AxisValue; }

	void SimulateMovement(float DeltaTime);


	

private:

	FVector GetAirResistance();



	// Player Physics
	void ApplyRotation(float DeltaTime, float MoveSideValue);
	void UpdateLocationFromVelocity(float DelatTime);
	

	UPROPERTY(EditAnywhere)
	float Mass = 100; //kg

	// force to character
	UPROPERTY(EditAnywhere)
	float MaxDriveForce = 1000;

	UPROPERTY(EditAnywhere)
	float DragCoefficient = 30;

	UPROPERTY()
	FVector Velocity;
	FVector Velocity2;

	float MoveForwardBackwardValue;
	float MoveSideVal;
};
