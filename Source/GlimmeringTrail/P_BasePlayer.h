// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EPlayerGameState.h"
#include "InputActionValue.h"
#include "../Plugins/CustomStateMachine/Source/CustomStateMachine/Public/PlayerStateComponent.h" 

#include "P_BasePlayer.generated.h"

UCLASS()
class GLIMMERINGTRAIL_API AP_BasePlayer : public APawn
{
	GENERATED_BODY()

public:

	AP_BasePlayer();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Manage player states
	//void ChangeState(EPlayerGameState NewState);

	void ChangePrimaryState(EPlayerMainState NewPrimaryState);
	void ChangeGroundedSubState(EPlayerGroundedSubState NewGroundedSubState);
	void ChangeAirborneSubState(EPlayerAirborneSubState NewAirborneSubState);

	// Current player state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainState")
	EPlayerMainState CurrMainState;
	
	// Current player state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GroundState")
	EPlayerGroundedSubState CurrGroundState;

	// Current player state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AirborneState")
	EPlayerAirborneSubState CurrAirborneState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	UPlayerStateComponent* StateManager;

	void DoubleJump();
	bool IsGrounded();
	
protected:
	virtual void BeginPlay() override;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPlayerMovementComponent* PlayerMoveComponent;

	

private:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* CapsuleComponent;

	void MoveForwardBackward(const FInputActionValue& Value);
	void StopMoveForwardBackward(const FInputActionValue& Value);
	void MoveLeftRight(const FInputActionValue& Value);
	void StopMoveLeftRight(const FInputActionValue& Value);

	void Jump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Valuee);
	
	//void DoubleJump();
	void WallJump();
	void WallSlide();
	void Dash();

	float CoyoteTime;
	float JumpBufferTime;

	float JumpKeyHoldTime = 0.0f;
	bool bJumpPressed = false;

	bool bCanDoubleJump;
	bool bIsWallJumping;
	bool bIsDashing;
	bool bIsRunning;

};
