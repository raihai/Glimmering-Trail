// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformCharacter.h"
#include "PlayerMovementComponent.h"
#include "GameFramework/Controller.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
APlatformCharacter::APlatformCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PlayerMoveComponent = CreateDefaultSubobject<UPlayerMovementComponent>(TEXT("MoveComponent"));

}

// Called when the game starts or when spawned
void APlatformCharacter::BeginPlay()
{
	Super::BeginPlay();
	//Add Input Mapping Context

	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerInputMap, 0);
		}
	}
	
}

// Called every frame
void APlatformCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlatformCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {




	}

}

