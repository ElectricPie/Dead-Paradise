// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathfindingGridVisualiser.generated.h"

class APathfindingGrid;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEADPARADISE_API UPathfindingGridVisualiser : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPathfindingGridVisualiser();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	friend class FPathfindingComponentVisualizer;
	
	UPROPERTY()
	APathfindingGrid* PathfindingGrid = nullptr;
};
