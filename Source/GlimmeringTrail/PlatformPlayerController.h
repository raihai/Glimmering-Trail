// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyPlayerInterface.h"
#include "InputActionValue.h"
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


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* PlayerInputMap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_JumpUp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_MoveForward;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_MoveSideway;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Run;

protected:
	// Called when the game starts or when spawned
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

	virtual FJUMPSIGNNATURE* GetJumpDelegate() override;
	virtual FFRONTBACKSIGNNATURE* GetFrontBackDelegate() override;
	virtual FSIDEWAYSIGNNATURE* GetSidewayDelegate() override;
	virtual FRUNSIGNNATURE* GetRunDelegate() override;
	virtual FSTOPSIGNAUTURE* GetMoveXYStopDelegate() override;
	
private:

	void PressJumpButton();
	void PressForwardBackwardButton(const FInputActionValue& Value);
	void PressSidewaysButtonButton(const FInputActionValue& Value);
	void PressRunButton();
	void ReleaseMovementButton();



	FJUMPSIGNNATURE JumpDelegate;
	FFRONTBACKSIGNNATURE ForwardBackwardDelegate;
	FSIDEWAYSIGNNATURE SidewayDelegate;
	FRUNSIGNNATURE RunDelegate;
	FSTOPSIGNAUTURE StopDelegate;
	
};
