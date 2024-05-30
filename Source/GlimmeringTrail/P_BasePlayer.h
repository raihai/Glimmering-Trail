// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "PlayerMovementComponent.h"
#include "P_BasePlayer.generated.h"


UCLASS()
class GLIMMERINGTRAIL_API AP_BasePlayer : public APawn
{
	GENERATED_BODY()

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* PlayerInputMap;
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_ForwardBackwardMove;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_LeftRightMove;

public:
	// Sets default values for this pawn's properties
	AP_BasePlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPlayerMovementComponent* PlayerMoveComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* CapsuleComponent;

	void MoveForwardBackward(const FInputActionValue& Value);
	void StopMoveForwardBackward(const FInputActionValue& Value);
	void MoveLeftRight(const FInputActionValue& Value);
	void StopMoveLeftRight(const FInputActionValue& Value);

	void Jump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Valuee);

};
