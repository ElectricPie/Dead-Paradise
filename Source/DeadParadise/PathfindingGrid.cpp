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

float UPathfindingGrid::GetNodeRadius()
{
	return NodeRadius;
}

FPathingNode* UPathfindingGrid::NodeFromWorldPoint(const FVector& WorldPosition) const
{
	if (!Grid)
	{
		UE_LOG(LogTemp, Warning, TEXT("NFWP returning null"));
	
		return new FPathingNode();
	}
	
	float PosX = (WorldPosition.X - GetOwner()->GetActorLocation().X + GridWorldSize.X * 0.5f) / NodeDiameter;
	float PoxY = (WorldPosition.Y - GetOwner()->GetActorLocation().Y + GridWorldSize.Y * 0.5f) / NodeDiameter;
	
	PosX = FMath::Clamp(PosX, 0, GridWorldSize.X - 1);
	PoxY = FMath::Clamp(PoxY, 0, GridWorldSize.Y - 1);

	const int32 NodeArrayX = FMath::FloorToInt32(PosX);
	const int32 NodeArrayY = FMath::FloorToInt32(PoxY);

	return &Grid[NodeArrayX*GridSizeY+NodeArrayY];
}

TArray<FPathingNode*> UPathfindingGrid::GetNeighbouringNodes(const FPathingNode* Node)
{
	TArray<FPathingNode*> NeighbourNodes = TArray<FPathingNode*>(nullptr, 0);
	
	if (!Grid)
	{
		return NeighbourNodes;
	}
	
	for (int32 X = -1; X <= 1; X++)
	{
		for (int32 Y = -1; Y <= 1; Y++)
		{
			if (X == 0 && Y == 0)
			{
				continue;
			}

			const int32 CheckX = Node->GetGridX() + X;
			const int32 CheckY = Node->GetGridY() + Y;
			
			if (CheckX >= 0 && CheckX < GridSizeX && CheckY >= 0 && CheckY < GridSizeY)
			{
				NeighbourNodes.Add(&Grid[CheckX*GridSizeY+CheckY]);
			}
		}
	}

	return NeighbourNodes;
}

void UPathfindingGrid::GenerateGrid()
{
	// Clear the grid to prevent anything accessing it while it is being generated
	Grid = nullptr;
	
	UE_LOG(LogTemp, Warning, TEXT("Generating grid sized %dx%d"), GridSizeX, GridSizeY);
	
	NodeDiameter = NodeRadius * 2;
	GridSizeX = FMath::RoundToInt32(GridWorldSize.X / NodeDiameter);
	GridSizeY = FMath::RoundToInt32(GridWorldSize.Y / NodeDiameter);
	
	FPathingNode* GeneratedGrid = new FPathingNode[GridSizeX*GridSizeY];
	const FVector WorldBottomLeft = GetOwner()->GetActorLocation() - FVector::ForwardVector * GridWorldSize.X / 2 - FVector::RightVector * GridWorldSize.Y / 2;
	
	for (int32 X = 0; X < GridSizeX; X++)
	{
		for (int32 Y = 0; Y < GridSizeY; Y++)
	 	{
			FVector WorldPoint = WorldBottomLeft + FVector::ForwardVector * (X * NodeDiameter + NodeRadius) + FVector::RightVector * (Y * NodeDiameter + NodeRadius);
			
			TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
			TArray<AActor*> IgnoreActors;
			TArray<AActor*> HitActors;

			bool bIsWalkable = true;
			
			UKismetSystemLibrary::SphereOverlapActors(this, WorldPoint, NodeRadius, TraceObjectTypes, nullptr, IgnoreActors, HitActors);

			// Check if the overlapping actors are unwalkable
			for (const AActor* Actor : HitActors)
			{
				// Stop checking actors when any are unwalkable
				if (Actor->ContainsDataLayer(UnwalkableDataLayer))
				{
					bIsWalkable = false;
					break;
				}
			}
			
			GeneratedGrid[X*GridSizeY+Y].SetupNode(bIsWalkable, WorldPoint, X, Y);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Grid finished generating"));
	Grid = GeneratedGrid;
}
