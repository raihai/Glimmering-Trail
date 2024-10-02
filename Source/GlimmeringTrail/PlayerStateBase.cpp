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

		PlayerController->GetJumpDelegate()->AddUObject(this, &UPlayerStateBase::PressJump);

	}

}

void UPlayerStateBase::TickState()
{
	Super::TickState();
}

void UPlayerStateBase::OnExitState()
{
	Super::OnExitState();
	PlayerController->GetJumpDelegate()->RemoveAll(this);


}

void UPlayerStateBase::PressJump()
{
}
