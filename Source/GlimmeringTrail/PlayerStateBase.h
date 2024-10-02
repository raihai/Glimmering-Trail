// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateBase.h"
#include "P_BasePlayer.h"
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
	class IMyPlayerInterface* PlayerController = nullptr;
	virtual void OnEnterState(AActor* OwnerRef) override;
	virtual void TickState() override;
	virtual void OnExitState() override;

protected:
	virtual void PressJump();

	
};
