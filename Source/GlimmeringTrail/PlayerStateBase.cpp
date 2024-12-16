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

	if (!PlayerController)
	{
		PlayerController = Cast<APlatformPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	}

	if (!PlayerInterface) {
		PlayerInterface = Cast<IMyPlayerInterface>(UGameplayStatics::GetPlayerController(this, 0));
	}
	
	if (PlayerInterface) {
		PlayerInterface->GetJumpDelegate()->AddUObject(this, &UPlayerStateBase::HandleJump); // add func to delegate
		PlayerInterface->GetRunDelegate()->AddUObject(this, &UPlayerStateBase::UPlayerStateBase::HandleRunning);

	}
}

void UPlayerStateBase::TickState(float DeltaTime)
{
	Super::TickState(DeltaTime);

}

void UPlayerStateBase::OnExitState()
{
	Super::OnExitState();
	if (PlayerInterface)
	{
		PlayerInterface->GetJumpDelegate()->RemoveAll(this);
		PlayerInterface->GetRunDelegate()->RemoveAll(this);
		
	}

}

void UPlayerStateBase::HandleJump()
{
	float g = (2 * 300) / (0.44 * 0.44);

	if (!PlayerRef->IsGrounded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot jump while already airborne"));
		return;
	}

	float jumpHeight = 300.0f; // Customize as needed
	float jumpVelocity = FMath::Sqrt(2 * g * jumpHeight);

	FVector horizontalVelocity = FVector::ZeroVector;
	
	if (PlayerInterface)
	{
		// Calculate horizontal velocity based on input values
		horizontalVelocity += PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 200.0f; // Adjust multiplier as needed
		horizontalVelocity += PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 200.0f;
	}

	FVector currVelocity = horizontalVelocity;
	currVelocity.Z = jumpVelocity;

	// Set the new velocity
	PlayerRef->PlayerMoveComponent->Velocity = currVelocity;

	//UE_LOG(LogTemp, Warning, TEXT("Jump initiated with velocity: %s"), *currVelocity.ToString());
	PlayerRef->StateManager->SwitchStateByKey("Air");
}


void UPlayerStateBase::HandleRunning()
{
	//run movemenetn
}


bool UPlayerStateBase::IsGrounded(FHitResult& HitResult)
{
	/*FHitResult hitResult;*/
	FVector startLocation = PlayerRef->GetActorLocation();
	FVector endLocation = startLocation - FVector(0, 0, 100.0f);
	float sphereRadius = 34.0f;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(PlayerRef); // Ignore the player itself in the raycast

	bool bHit = GetWorld()->SweepSingleByChannel(HitResult,
		startLocation,
		endLocation,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(sphereRadius),
		CollisionParams)
		;

	/*DrawDebugSphere(GetWorld(), startLocation, sphereRadius, 12, FColor::Blue, false, 0.5f);
	DrawDebugSphere(GetWorld(), endLocation, sphereRadius, 12, FColor::Red, false, 0.5f);*/
	//DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, 10.0f, 0, 5.0f);

	return bHit;

}

bool UPlayerStateBase::SlopeCheck(FVector& ImpactNormal)
{
	// find the angle between the player down vec and impact surface normal 

	FVector playerUpVec = PlayerRef->GetActorUpVector().GetSafeNormal();
	FVector impactNormal = ImpactNormal.GetSafeNormal();
	float cosValue = FVector::DotProduct(playerUpVec, impactNormal);

	double angleInDegree = FMath::RadiansToDegrees(acos(cosValue));

	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, FString::Printf(TEXT("Slope angle: %f"), angleInDegree));

	if (angleInDegree >= 30.0f && angleInDegree < 85) {

		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("Too Steep, sliding down"));
		return true;
	}

	return false;
}
