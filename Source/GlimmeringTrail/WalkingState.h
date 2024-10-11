// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStateBase.h"
#include "WalkingState.generated.h"

/**
 * 
 */
UCLASS()
class GLIMMERINGTRAIL_API UWalkingState : public UPlayerStateBase
{
	GENERATED_BODY()

public:

protected:
	virtual void OnEnterState(AActor* OwnerRef) override;
	virtual void TickState(float DeltaTime) override;
	virtual void HandleForwardBackwardMovement(const FInputActionValue& Value) override;
	virtual void HandleSidewayMovment(const FInputActionValue& Value) override;
	virtual void HandleStopXYMovment() override;
	virtual void HandleJump() override;

private:

	FVector GetAirResistance();

	float WalkForce = 300;
	float FrontBackValue = 0;
	float SideValue = 0;
	float DragCoefficient = 25;
	FVector m_Velocity;

	bool bQuickStop = false;
	
};
