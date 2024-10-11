// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EPlayerGameState.h"
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

	void SetMoveForwardValue(float AxisValue);
	void SetMoveSidewayValue(float AxisValue);

	void StopMovement();
	//void HandleMovement(EPlayerGameState PlayerState, float DeltaTime);
	void HandleJumping(float DeltaTime);

	UPROPERTY()
	FVector Velocity;
private:

	void SimulateMovement(float DeltaTime, float MaxForce);
	void HandleWalking(float DeltaTime);
	void HandleRunning(float DeltaTime);
	//void HandleJumping(float DeltaTime);
	void HandleFalling(float DeltaTime);

	//helper func
	FVector GetAirResistance();

	void ApplyRotation(float DeltaTime, float MoveSideValue);
	void UpdateLocationFromVelocity(float DelatTime);
	
	UPROPERTY(EditAnywhere)
	float Mass = 50; 

	UPROPERTY(EditAnywhere)
	float DragCoefficient = 25;

	bool bQuickStop = false;

	float Gravity = 100.0;
	float JumpHeight = 300;
	FVector InitJumpVelocity = FVector(0, 0, 100);
	float testJumpVelocity = 100;

	/*UPROPERTY()
	FVector Velocity;*/
	
	float MoveForwardBackwardValue;
	float MoveSideVal;

	
};
