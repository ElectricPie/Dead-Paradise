// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RtsPlayerController.generated.h"

class USelectableComponent;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()	
class DEADPARADISE_API ARtsPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* SelectAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float SelectionRaycastDistance = 2000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Selection")
	AActor* SelectionMarker;

	// Using a array for when multi select is implemented
	UPROPERTY()
	TArray<USelectableComponent*> SelectedObjects;

	bool RaycastToMouse(FVector& HitLocation, AActor*& HitActor);
	
	void Select();

	void MoveSelected();
};
