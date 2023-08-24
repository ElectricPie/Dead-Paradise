// Fill out your copyright notice in the Description page of Project Settings.


#include "RtsPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SelectableComponent.h"

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
		EnhancedInputComponent->BindAction(SetTargetAction, ETriggerEvent::Triggered, this, &ARtsPlayerController::MoveSelected);
	}
}

bool ARtsPlayerController::RaycastToMouse(OUT FVector& HitLocation, OUT AActor*& HitActor)
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
			HitActor = HitResult.GetActor();
			HitLocation = HitResult.ImpactPoint;
			return true;
		}
	}

	return false;
}

// TODO: Implement method for selecting multiple
void ARtsPlayerController::Select()
{
	// Clear the selected objects
	SelectedObjects.Empty();
	
	int32 ViewportSizeX;
	int32 ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	AActor* HitActor;
	FVector HitLocation;
	if (RaycastToMouse(HitLocation, HitActor))
	{
		if (USelectableComponent* Selectable = HitActor->GetComponentByClass<USelectableComponent>())
		{
			Selectable->Select();
			// Keep track of what objects are selected
			SelectedObjects.Add(Selectable);
		}
	}
	else
	{
		SelectedObjects.Empty();
	}
}

void ARtsPlayerController::MoveSelected()
{
	AActor* HitActor;
	FVector HitLocation;
	if (RaycastToMouse(HitLocation, HitActor))
	{
		for (int32 i = 0; i < SelectedObjects.Num(); i++)
		{
			SelectedObjects[i]->SetTarget(HitLocation);
		}
	}
}
