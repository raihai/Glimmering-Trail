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

	float jumpHeight = 300.0f;
	float jumpVelocity = FMath::Sqrt(2 * g * jumpHeight);

	FVector horizontalVelocity = FVector::ZeroVector;
	
	if (PlayerInterface)
	{
		
		horizontalVelocity += PlayerRef->GetActorForwardVector() * PlayerController->CurrentFrontBackValue * 200.0f; 
		horizontalVelocity += PlayerRef->GetActorRightVector() * PlayerController->CurrentSideValue * 200.0f;
	}

	FVector currVelocity = horizontalVelocity;
	currVelocity.Z = jumpVelocity;

	PlayerRef->PlayerMoveComponent->Velocity = currVelocity;

	PlayerRef->StateManager->SwitchStateByKey("Air");
}


void UPlayerStateBase::HandleRunning()
{
	//run movemenetn
}


bool UPlayerStateBase::IsGrounded(FHitResult& HitResult)
{
	//FVector playerLocation = PlayerRef->GetActorLocation();
	//FVector startLocation = playerLocation;
	//FVector endLocation = playerLocation - FVector(0, 0, 110.0f); // Slightly below capsule

	//FCollisionQueryParams CollisionParams;
	//CollisionParams.AddIgnoredActor(PlayerRef);

	//bool bHit = GetWorld()->LineTraceSingleByChannel(
	//	HitResult,
	//	startLocation,
	//	endLocation,
	//	ECC_Visibility,
	//	CollisionParams
	//);

	//DrawDebugLine(GetWorld(), startLocation, endLocation, bHit ? FColor::Green : FColor::Red, false, 1.0f, 0, 1.0f);

	//return bHit && FVector::DotProduct(HitResult.ImpactNormal, PlayerRef->GetActorUpVector()) > 0.86f; 

	FVector playerLocation = PlayerRef->GetActorLocation();

	
	FVector startLocation = playerLocation -  FVector(0, 0, 80.0f);
	FVector endLocation = startLocation - FVector(0, 0, 20.0f); 

	FVector BoxExtent(40.0f, 40.0f, 10.0f);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(PlayerRef);

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		startLocation,
		endLocation,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeBox(BoxExtent),
		CollisionParams
	);

	
	//DrawDebugBox(
	//	GetWorld(),
	//	(startLocation + endLocation) * 0.5f, 
	//	BoxExtent,
	//	FQuat::Identity,
	//	bHit ? FColor::Green : FColor::Red,
	//	false,
	//	1.0f
	//);

	return bHit;
}

bool UPlayerStateBase::IsGroundedRay(FHitResult& HitResult)
{
	FVector playerLocation = PlayerRef->GetActorLocation();
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(PlayerRef); 

	FVector startLocation = playerLocation;
	FVector endLocation = startLocation - FVector(0, 0, 110.0f);
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,		
		startLocation,	
		endLocation,
		ECC_Visibility,
		CollisionParams
		);

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
