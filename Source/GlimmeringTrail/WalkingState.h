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

	virtual void OnExitState() override;

private:

	FVector GetAirResistance();

	float WalkForce = 30000;
	float DragCoefficient = 25;

	FVector m_Velocity;
	
	bool bQuickStop = false;

	
};
