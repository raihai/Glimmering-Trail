// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStateBase.h"
#include "GroundedState.generated.h"

/**
 * 
 */
UCLASS()
class GLIMMERINGTRAIL_API UGroundedState : public UPlayerStateBase
{
	GENERATED_BODY()

	//SuperState
	// Handle all states that can happen during grounded state
	
public:

protected:
	virtual void HandleJump() override;
	virtual void OnEnterState(AActor* OwnerRef) override;
	virtual void TickState(float DeltaTime) override;
	virtual void OnExitState() override;



private:

};
