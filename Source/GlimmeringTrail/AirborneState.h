// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStateBase.h"
#include "AirborneState.generated.h"

/**
 * 
 */
UCLASS()
class GLIMMERINGTRAIL_API UAirborneState : public UPlayerStateBase
{
	GENERATED_BODY()

public:

protected:
	virtual void HandleJump() override;
	
	virtual void OnEnterState(AActor* OwnerRef);
	virtual void TickState(float DeltaTime) override;
	virtual void OnExitState() override;

private:
	void IsFallingFromLedge(FHitResult& hitResult, float deltaTime);
	void FallingMovement(FHitResult& HitResult, float DelaTime);
	bool OnEdgeCheck(FHitResult& hitres);
	


	FVector m_Velocity;
	FVector m_LedgeNormal;
	float gravity = 2 * 300 * 5.165;
	bool bIsFallingFromLedge;
	bool bSlopeEdge;

	float maxHorizontalHeight = 700.0f;

	float GroundCheckDelay = 0.2f;
	float ElapsedTimeInAirborne = 0.0f;
	
};
