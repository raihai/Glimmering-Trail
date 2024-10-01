// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPlayerInterface.h"
#include "GameFramework/PlayerController.h"
#include "PlatformPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GLIMMERINGTRAIL_API APlatformPlayerController : public APlayerController, public IMyPlayerInterface
{
	GENERATED_BODY()

public:
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;

	void PressJump();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual FJUMPSIGNNATURE* GetJumpDelegate() override;


private:
	FJUMPSIGNNATURE JumpDelegate;
	
};
