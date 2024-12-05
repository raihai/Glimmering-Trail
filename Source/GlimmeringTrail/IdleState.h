// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStateBase.h"
#include "IdleState.generated.h"

/**
 * 
 */
UCLASS()
class GLIMMERINGTRAIL_API UIdleState : public UPlayerStateBase
{
	GENERATED_BODY()

public:

protected:
	/*virtual void HandleJump() override;*/
	virtual void OnEnterState(AActor* OwnerRef) override;
	//virtual void HandleForwardBackwardMovement(const FInputActionValue& Value) override;
	//virtual void HandleSidewayMovement(const FInputActionValue& Value) override;
	//virtual void HandleStopForwardMovement() override;
	//virtual void HandleStopSideMovement() override;
	virtual void TickState(float DeltaTime) override;
	virtual void OnExitState() override;


private:
	float m_FValue = 0;
	float m_SValue = 0;
	
};
