// Fill out your copyright notice in the Description page of Project Settings.


#include "RtsPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ARtsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		InputSubsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void ARtsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &ARtsPlayerController::Select);
	}
}

void ARtsPlayerController::Select()
{
	int32 ViewportSizeX;
	int32 ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	// Get the screen position of the mouse
	if (FVector2D MouseScreenLocation; GetMousePosition(MouseScreenLocation.X, MouseScreenLocation.Y))
	{
		// Get the direction of the mouse
		FVector WorldPosition;
		FVector WorldDirection;
		DeprojectScreenPositionToWorld(MouseScreenLocation.X, MouseScreenLocation.Y, WorldPosition, WorldDirection);

		// Raycast
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldPosition,
			WorldPosition + WorldDirection * SelectionRaycastDistance, ECC_Visibility, QueryParams))
		{
			if (AActor* HitActor = HitResult.GetActor())
			{
				UE_LOG(LogTemp, Warning, TEXT("Selected %s"), *HitActor->GetActorLabel());
			}
		}
		
	}
}
