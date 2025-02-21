// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerInterface.h"
#include "Components/CapsuleComponent.h"


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
	
}

void UPlayerStateBase::ClampHorizontalVelocity(FVector& Velocity, float MaxHorizontalSpeed)
{
	FVector HorizontalVelocity = FVector(Velocity.X, Velocity.Y, 0.0f); 
	float HorizontalSpeed = HorizontalVelocity.Size(); 

	if (HorizontalSpeed > MaxHorizontalSpeed) {
		HorizontalVelocity = HorizontalVelocity.GetSafeNormal() * MaxHorizontalSpeed; 
		Velocity.X = HorizontalVelocity.X;
		Velocity.Y = HorizontalVelocity.Y;
	}
}

void UPlayerStateBase::ApplyFloatingEffect(FHitResult& GroundHit, float DeltaTime)
{
	const float DesiredHoverHeight = 200.0f;  
	const float Stiffness = 200.0f;          
	const float Damping = 100.0f;           
	const float MaxHoverForce = 2000.0f;     

	FVector currentVelocity = PlayerRef->PlayerMoveComponent->Velocity;
	float currentHeight = (PlayerRef->GetActorLocation().Z - GroundHit.Location.Z);
	float hoverError = DesiredHoverHeight - currentHeight;

	float hoverForce = 100.0f;

	if (hoverError > 0)
	{
		hoverForce = (Stiffness * hoverError) - (Damping * currentVelocity.Z);
		hoverForce = FMath::Clamp(hoverForce, 0.0f, MaxHoverForce);
	}

	FVector floatingForce = GroundHit.ImpactNormal.GetSafeNormal() * hoverForce;

	PlayerRef->PlayerMoveComponent->Velocity += floatingForce * DeltaTime;

	UE_LOG(LogTemp, Warning, TEXT("Floating force applied: %s"), *floatingForce.ToString());


	
	if (GroundHit.bBlockingHit) {
		FVector currVelocity = PlayerRef->PlayerMoveComponent->Velocity;
		FVector down
	}
}


bool UPlayerStateBase::IsGrounded(FHitResult& HitResult)
{
	float CapsuleHalfHeight = 90.0f;
	FVector StartLocation = PlayerRef->GetActorLocation() - FVector(0, 0, 120);
	FVector EndLocation = StartLocation - FVector(0.0f, 0.0f, 20.0f); 
	float SphereRadius = 30.0f;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(PlayerRef);

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(SphereRadius),
		CollisionParams
	);

	DrawDebugSphere(GetWorld(), EndLocation, SphereRadius, 16, bHit ? FColor::Green : FColor::Red, false, 1.0f);

	float VerticalVelocity = PlayerRef->PlayerMoveComponent->Velocity.Z;
	return bHit && VerticalVelocity <= 0.1f; 
}

bool UPlayerStateBase::IsGroundedRay(FHitResult& HitResult)
{
	FVector startLocation = PlayerRef->GetActorLocation() - FVector(0.0f, 0.0f, 90.0f);
	FVector endLocation = startLocation - FVector(0, 0, 100.0f);
	

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(PlayerRef); // Ignore the player itself in the raycast

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult,
		startLocation,
		endLocation,
		ECC_WorldDynamic,
		CollisionParams)
		;

	DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, 10.0f);

	return bHit;
}

bool UPlayerStateBase::IsGroundedOnSlope(FHitResult& HitResult)
{
	FVector playerLocation = PlayerRef->GetActorLocation();
	
	
	FVector startLocation = PlayerRef->GetActorLocation();
	FVector endLocation = startLocation - FVector(0, 0, 100.0f);
	float sphereRadius = 35.0f;

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

	DrawDebugSphere(GetWorld(), startLocation, sphereRadius, 12, FColor::Blue, false, 0.5f);
	DrawDebugSphere(GetWorld(), endLocation, sphereRadius, 12, FColor::Red, false, 0.5f);

	if (bHit && SlopeCheck(HitResult.ImpactNormal))
	{
		// slope is too steep
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("Steep slope detected in Grounded On Slope Check."));
		return true;
	}

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
