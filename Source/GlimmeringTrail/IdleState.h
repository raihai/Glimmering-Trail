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
	virtual void PressJump() override;
	virtual void TickState() override;
	
};
