// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingGrid.h"
#include "PathingNode.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UPathfindingGrid::UPathfindingGrid()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UPathfindingGrid::BeginPlay()
{
	Super::BeginPlay();

	// ...
	GenerateGrid();
}

// Called every frame
void UPathfindingGrid::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPathfindingGrid::GenerateGrid()
{
	UE_LOG(LogTemp, Warning, TEXT("Generating grid sized %dx%d"), GridSizeX, GridSizeY);
	
	NodeDiameter = NodeRadius * 2;
	GridSizeX = FGenericPlatformMath::RoundToInt(GridWorldSize.X / NodeDiameter);
	GridSizeY = FGenericPlatformMath::RoundToInt(GridWorldSize.Y / NodeDiameter);
	
	Grid = new PathingNode[GridSizeX*GridSizeY];
	FVector WorldBottomLeft = GetOwner()->GetActorLocation() - FVector::ForwardVector * GridWorldSize.X / 2 - FVector::RightVector * GridWorldSize.Y / 2;
	
	for (int X = 0; X < GridSizeX; X++)
	{
		for (int Y = 0; Y < GridSizeY; Y++)
	 	{
			FVector WorldPoint = WorldBottomLeft + FVector::ForwardVector * (X * NodeDiameter + NodeRadius) + FVector::RightVector * (Y * NodeDiameter + NodeRadius);
			
			TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
			TArray<AActor*> IgnoreActors;
			TArray<AActor*> HitActors;
			
			bool bIsWalkable = !UKismetSystemLibrary::SphereOverlapActors(this, WorldPoint, NodeRadius, TraceObjectTypes, nullptr, IgnoreActors, HitActors);
			
			FColor WalkableColor = bIsWalkable ? FColor::Green : FColor::Red;
			DrawDebugBox(GetWorld(), WorldPoint, FVector(NodeRadius), WalkableColor, false, 10.f);
			
			Grid[X*GridSizeY+Y].SetupNode(bIsWalkable, WorldPoint);
		}
	}
}

