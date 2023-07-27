// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingGridVisualiser.h"

#include "PathfindingGrid.h"


// Sets default values for this component's properties
UPathfindingGridVisualiser::UPathfindingGridVisualiser()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	PathfindingGrid = Cast<APathfindingGrid>(GetOwner());
}


// Called when the game starts
void UPathfindingGridVisualiser::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPathfindingGridVisualiser::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

