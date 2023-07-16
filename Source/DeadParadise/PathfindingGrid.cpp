// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingGrid.h"
#include "PathingNode.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APathfindingGrid::APathfindingGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APathfindingGrid::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateGrid();
}

// Called every frame
void APathfindingGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


float APathfindingGrid::GetNodeRadius() const
{
	return NodeRadius;
}

int APathfindingGrid::GetGridSize() const
{
	return GridSizeX * GridSizeY;
}

FPathingNode* APathfindingGrid::NodeFromWorldPoint(const FVector& WorldPosition) const
{
	if (!Grid)
	{
		UE_LOG(LogTemp, Warning, TEXT("NFWP returning null"));
	
		return new FPathingNode();
	}
	
	float PosX = (WorldPosition.X - GetActorLocation().X + GridWorldSize.X * 0.5f) / NodeDiameter;
	float PoxY = (WorldPosition.Y - GetActorLocation().Y + GridWorldSize.Y * 0.5f) / NodeDiameter;
	
	PosX = FMath::Clamp(PosX, 0, GridWorldSize.X - 1);
	PoxY = FMath::Clamp(PoxY, 0, GridWorldSize.Y - 1);

	const int32 NodeArrayX = FMath::FloorToInt32(PosX);
	const int32 NodeArrayY = FMath::FloorToInt32(PoxY);

	return &Grid[NodeArrayX*GridSizeY+NodeArrayY];
}

TArray<FPathingNode*> APathfindingGrid::GetNeighbouringNodes(const FPathingNode* Node)
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

void APathfindingGrid::GenerateGrid()
{
	// Clear the grid to prevent anything accessing it while it is being generated
	Grid = nullptr;
	
	NodeDiameter = NodeRadius * 2;
	GridSizeX = FMath::RoundToInt32(GridWorldSize.X / NodeDiameter);
	GridSizeY = FMath::RoundToInt32(GridWorldSize.Y / NodeDiameter);
	
	UE_LOG(LogTemp, Warning, TEXT("Generating grid sized %dx%d"), GridSizeX, GridSizeY);
	
	FPathingNode* GeneratedGrid = new FPathingNode[GridSizeX*GridSizeY];
	const FVector WorldBottomLeft = GetActorLocation() - FVector::ForwardVector * GridWorldSize.X / 2 - FVector::RightVector * GridWorldSize.Y / 2;
	
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
