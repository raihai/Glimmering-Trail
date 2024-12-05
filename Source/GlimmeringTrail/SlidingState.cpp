// Fill out your copyright notice in the Description page of Project Settings.


#include "SlidingState.h"

void USlidingState::OnEnterState(AActor* OwnerRef)
{
	Super::OnEnterState(OwnerRef);
	bCurrentlySliding = true;
}

void USlidingState::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);

	// need vector perpendicular to the surface normal

	FHitResult hitResult;
	IsGrounded(hitResult);
	FVector surfaceNormal = hitResult.ImpactNormal;
	FVector gravity = PlayerRef->GetActorUpVector();
	FVector slideDirection = gravity - (FVector::DotProduct(gravity, surfaceNormal) / (surfaceNormal.Size() * surfaceNormal.Size())) * surfaceNormal; // vector projection onto plane
	slideDirection.Normalize();

	if (TestFunc(surfaceNormal)) {
		DrawDebugLine(GetWorld(), hitResult.Location, hitResult.Location + (-slideDirection * 200), FColor::Purple, false, 10.0f, 0, 1.0f);
		
		//PlayerRef->PlayerMoveComponent->Velocity = FVector::ZeroVector;
	
		FVector Translation = -slideDirection * DeltaTime * 200;
		PlayerRef->PlayerMoveComponent->Velocity = Translation;

		PlayerRef->AddActorWorldOffset(Translation, true, &hitResult);

	}
	else {
		PlayerRef->PlayerMoveComponent->Velocity = FVector::ZeroVector;
		PlayerRef->StateManager->SwitchStateByKey("Grounded");
	}


}

//void USlidingState::HandleForwardBackwardMovement(const FInputActionValue& Value)
//{
//	Super::HandleForwardBackwardMovement(Value);
//	FrontBackValue = Value.Get<float>();
//}
//
//void USlidingState::HandleSidewayMovement(const FInputActionValue& Value)
//{
//	Super::HandleForwardBackwardMovement(Value);
//	FrontBackValue = Value.Get<float>();
//}

//void USlidingState::HandleStopXYMovment()
//{
//	bQuickStop = true;
//	FrontBackValue = 0;
//	SideValue = 0;
//}
//
//void USlidingState::HandleJump()
//{
//	Super::HandleJump();
//	PlayerRef->StateManager->SwitchStateByKey("Air");
//
//}

bool USlidingState::TestFunc(FVector& Normal)
{
	FVector playerUpVec = PlayerRef->GetActorUpVector();
	float cosValue = FVector::DotProduct(playerUpVec, Normal) / (playerUpVec.Size() * Normal.Size());

	double angleInDegree = FMath::RadiansToDegrees(acos(cosValue));

	if (angleInDegree > 10.0f) {

		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("No walkable surface detected, sliding down"));
		return true;
	}
	return false;

}
