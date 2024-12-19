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
	virtual void OnExitState() override;
	

private:

	void MovePlayerAlongSlope(const FVector& SurfaceNormal, FHitResult& HitRes, float dt); 
	
};
