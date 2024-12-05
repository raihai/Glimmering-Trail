// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStateBase.h"
#include "SlidingState.generated.h"

/**
 * 
 */
UCLASS()
class GLIMMERINGTRAIL_API USlidingState : public UPlayerStateBase
{
	GENERATED_BODY()

public:

protected:
	virtual void OnEnterState(AActor* OwnerRef) override;
	virtual void TickState(float DeltaTime) override;
	/*virtual void HandleForwardBackwardMovement(const FInputActionValue& Value) override;
	virtual void HandleSidewayMovement(const FInputActionValue& Value) override;
	virtual void HandleStopXYMovment() override;
	virtual void HandleJump() override;*/

private:

	bool TestFunc(FVector& Normal); 
	//float FrontBackValue = 0;
//	float SideValue = 0;
	float DragCoefficient = 25;
	FVector m_Velocity;
	float g = (2 * 300) / (0.44 * 0.44);
	bool bQuickStop = false;

	bool bCurrentlySliding = false;
	
};
