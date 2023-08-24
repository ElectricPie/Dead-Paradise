// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCamera.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCamera::APlayerCamera()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComponent;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void APlayerCamera::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	// Get the game viewport client
	ViewportClient = GetWorld()->GetGameViewport();
}

void APlayerCamera::UnlockCamera(const FInputActionValue& Value)
{
	bCameraIsUnlocked = Value.Get<bool>();

	if (!PlayerController) return;
	PlayerController->SetShowMouseCursor(!bCameraIsUnlocked);

	// Store the cursors position before unlocking the camera and return it to that position once camera is locked
	if (bCameraIsUnlocked)
	{
		PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);
	}
	else
	{
		PlayerController->SetMouseLocation(MousePosition.X, MousePosition.Y);
	}
}

void APlayerCamera::MoveCamera(const FInputActionValue& Value)
{
	// Only allow the camera to move if the unlock camera button is held down
	if (bCameraIsUnlocked)
	{
		const FVector MoveAxisValue = Value.Get<FVector>();

		const FVector DeltaLocation = MoveAxisValue * MovementSpeedModifier * UGameplayStatics::GetWorldDeltaSeconds(this);
		AddActorLocalOffset(DeltaLocation, true);
		
		PlayerController->SetMouseLocation(MousePosition.X, MousePosition.Y);
	}
}

// Called every frame
void APlayerCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Centre the mouse cursor so it doesn't hit the viewport borders and stop the mouse from moving
	if (bCameraIsUnlocked)
	{
		// Get the viewport size
		FVector2d ViewportSize;
		ViewportClient->GetViewportSize(ViewportSize);
		PlayerController->SetMouseLocation(ViewportSize.X / 2, ViewportSize.Y / 2);
	}
}

// Called to bind functionality to input
void APlayerCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* PlayerEnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		PlayerEnhancedInputComponent->BindAction(UnlockCameraAction, ETriggerEvent::Triggered, this, &APlayerCamera::UnlockCamera);
		PlayerEnhancedInputComponent->BindAction(MoveCameraAction, ETriggerEvent::Triggered, this, &APlayerCamera::MoveCamera);
	}
}

