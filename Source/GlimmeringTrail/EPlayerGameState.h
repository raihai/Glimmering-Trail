
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EPlayerMainState: uint8
{
	Grounded,
	Airborne
};

UENUM(BlueprintType)
enum class EPlayerGroundedSubState : uint8
{
	Idle,
	Walking,
	Running,
	Dash
};

UENUM(BlueprintType)
enum class EPlayerAirborneSubState : uint8
{
	Jumping,
	Falling,
	DoubleJumping,
	WallJump,
	
};








