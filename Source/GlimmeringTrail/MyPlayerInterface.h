// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "UObject/Interface.h"

#include "MyPlayerInterface.generated.h"


DECLARE_MULTICAST_DELEGATE(FJUMPSIGNNATURE);
DECLARE_MULTICAST_DELEGATE_OneParam(FFRONTBACKSIGNNATURE, const FInputActionValue& Value);
DECLARE_MULTICAST_DELEGATE_OneParam(FSIDEWAYSIGNNATURE,  const FInputActionValue& Value);
DECLARE_MULTICAST_DELEGATE(FRUNSIGNNATURE);
DECLARE_MULTICAST_DELEGATE(FSTOPFORWARDSIGNAUTURE);
DECLARE_MULTICAST_DELEGATE(FSTOPSIDESIGNAUTURE);
//DECLARE_MULTICAST_DELEGATE(FJUMPSIGNNATURE);



// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMyPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GLIMMERINGTRAIL_API IMyPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual FJUMPSIGNNATURE* GetJumpDelegate() = 0;
	virtual FFRONTBACKSIGNNATURE* GetFrontBackDelegate() = 0;
	virtual FSIDEWAYSIGNNATURE* GetSidewayDelegate() = 0; 
	virtual FRUNSIGNNATURE* GetRunDelegate() = 0;
	virtual FSTOPFORWARDSIGNAUTURE* GetMoveForwardStopDelegate() = 0;
	virtual FSTOPSIDESIGNAUTURE* GetMoveSideStopDelegate() = 0;
};
