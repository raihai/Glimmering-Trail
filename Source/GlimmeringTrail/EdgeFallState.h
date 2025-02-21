// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStateBase.h"
#include "EdgeFallState.generated.h"

/**
 * 
 */
UCLASS()
class GLIMMERINGTRAIL_API UEdgeFallState : public UPlayerStateBase
{
	GENERATED_BODY()

public:

protected:
	virtual void HandleJump() override;

	virtual void OnEnterState(AActor* OwnerRef);
	virtual void TickState(float DeltaTime) override;
	virtual void OnExitState() override;

private:
	void FallingFromLedge(FHitResult& hitResult, float deltaTime);

	FVector m_Velocity;
	FVector m_LedgeNormal;
	float gravity = 2 * 300 * 5.165;
	bool bIsFallingFromLedge;
	bool bSlopeEdge;

	float maxHorizontalHeight = 700.0f;

	float GroundCheckDelay = 0.2f;
	float ElapsedTimeInAirborne = 0.0f;
};
