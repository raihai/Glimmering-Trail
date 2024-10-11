// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerInterface.h"

void UPlayerStateBase::OnEnterState(AActor* OwnerRef)
{
	Super::OnEnterState(OwnerRef);

	if (!PlayerRef) {
		PlayerRef = Cast<AP_BasePlayer>(OwnerRef);
	}

	if (!PlayerController) {
		PlayerController = Cast<IMyPlayerInterface>(UGameplayStatics::GetPlayerController(this, 0));
	}
	
	if (PlayerController) {
		PlayerController->GetJumpDelegate()->AddUObject(this, &UPlayerStateBase::HandleJump); // add func to delegate
		PlayerController->GetFrontBackDelegate()->AddUObject(this, &UPlayerStateBase::UPlayerStateBase::HandleForwardBackwardMovement);
		PlayerController->GetSidewayDelegate()->AddUObject(this, &UPlayerStateBase::UPlayerStateBase::HandleSidewayMovment);
		PlayerController->GetRunDelegate()->AddUObject(this, &UPlayerStateBase::UPlayerStateBase::HandleRunning);
		PlayerController->GetMoveXYStopDelegate()->AddUObject(this, &UPlayerStateBase::UPlayerStateBase::HandleStopXYMovment);
	}

}

void UPlayerStateBase::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);

	
}

void UPlayerStateBase::OnExitState()
{
	Super::OnExitState();
	PlayerController->GetJumpDelegate()->RemoveAll(this);
	PlayerController->GetFrontBackDelegate()->RemoveAll(this);
	PlayerController->GetSidewayDelegate()->RemoveAll(this);
	PlayerController->GetRunDelegate()->RemoveAll(this);


}

void UPlayerStateBase::HandleJump()
{
	// Jump code
}

void UPlayerStateBase::HandleForwardBackwardMovement(const FInputActionValue& Value)
{
	// forward/backward movement 
}

void UPlayerStateBase::HandleSidewayMovment(const FInputActionValue& Value)
{
	// side movement
}

void UPlayerStateBase::HandleRunning()
{
	//run movemenetn
}

void UPlayerStateBase::HandleStopXYMovment()
{
	// handle stop movement
}
