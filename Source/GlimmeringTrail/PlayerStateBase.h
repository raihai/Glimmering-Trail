// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateBase.h"
#include "P_BasePlayer.h"
#include "PlayerMovementComponent.h"
#include "PlatformPlayerController.h"
#include "PlayerStateBase.generated.h"

/**
 * 
 */
UCLASS()
class GLIMMERINGTRAIL_API UPlayerStateBase : public UStateBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	AP_BasePlayer* PlayerRef = nullptr;
	class IMyPlayerInterface* PlayerInterface = nullptr;
	APlatformPlayerController* PlayerController = nullptr;
	virtual void OnEnterState(AActor* OwnerRef) override;
	virtual void TickState(float DeltaTime) override;
	virtual void OnExitState() override;

protected:
	virtual void HandleJump();

	virtual void HandleRunning();
	virtual void ClampHorizontalVelocity(FVector& Velocity, float MaxHorizontalSpeed);
	virtual void ApplyFloatingEffect(FHitResult& HitResult, float DeltaTime);
	
	virtual bool IsGrounded(FHitResult& HitResult);
	virtual bool IsGroundedOnSlope(FHitResult& HitResult);
	virtual bool SlopeCheck(FVector& ImpactNormal); 
	virtual bool IsGroundedRay(FHitResult& HitResult);

	float FrontBackValue = 0;
	float SideValue = 0;
	bool FallingFromLedge = false;
};
