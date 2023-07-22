// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingGrid.h"

#include "PathfindingTerrain.h"
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

bool APathfindingGrid::NodeFromWorldPoint(const FVector& WorldPosition, OUT FPathingNode*& OutNode) const
{
	if (Grid.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("NFWP returning null"));
	
		return false;
	}
	
	float PosX = (WorldPosition.X - GetActorLocation().X + GridWorldSize.X * 0.5f) / NodeDiameter;
	float PoxY = (WorldPosition.Y - GetActorLocation().Y + GridWorldSize.Y * 0.5f) / NodeDiameter;
	
	PosX = FMath::Clamp(PosX, 0, GridWorldSize.X - 1);
	PoxY = FMath::Clamp(PoxY, 0, GridWorldSize.Y - 1);

	const int32 NodeArrayX = FMath::FloorToInt32(PosX);
	const int32 NodeArrayY = FMath::FloorToInt32(PoxY);

	OutNode = Grid[NodeArrayX*GridSizeY+NodeArrayY];
	return true;
}

bool APathfindingGrid::GetNeighbouringNodes(const FPathingNode& Node, OUT TArray<FPathingNode*>& OutNeighboringNodes)
{
	if (Grid.IsEmpty())
	{
		return false;
	}
	
	for (int32 X = -1; X <= 1; X++)
	{
		for (int32 Y = -1; Y <= 1; Y++)
		{
			if (X == 0 && Y == 0)
			{
				continue;
			}

			const int32 CheckX = Node.GetGridX() + X;
			const int32 CheckY = Node.GetGridY() + Y;
			
			if (CheckX >= 0 && CheckX < GridSizeX && CheckY >= 0 && CheckY < GridSizeY)
			{
				OutNeighboringNodes.Add(Grid[CheckX*GridSizeY+CheckY]);
			}
		}
	}

	return true;
}

void APathfindingGrid::GenerateGrid()
{
	// Clear the grid to prevent anything accessing it while it is being generated
	Grid.Empty();
	
	NodeDiameter = NodeRadius * 2;
	GridSizeX = FMath::RoundToInt32(GridWorldSize.X / NodeDiameter);
	GridSizeY = FMath::RoundToInt32(GridWorldSize.Y / NodeDiameter);
	
	UE_LOG(LogTemp, Warning, TEXT("Generating grid sized %dx%d"), GridSizeX, GridSizeY);
	
	TArray<FPathingNode*> GeneratedGrid;
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

			int32 MovementPenalty = 0;

			// Check if the overlapping actors have specific dataloader
			for (const AActor* Actor : HitActors)
			{
				// Stop checking actors when any are unwalkable
				if (Actor->ContainsDataLayer(UnwalkableDataLayer))
				{
					bIsWalkable = false;
					break;
				}

				// the hit actor 
				for (const TWeakObjectPtr<UPathfindingTerrain> Region : TerrainRegions)
				{
					if (Actor->ContainsDataLayer(Region.Get()->TerrainDataLayer.Get()))
					{
						MovementPenalty = Region.Get()->MovementPenalty;
					}
				}
			}

			// Assign a movement penalty depending on the terrain below the node
			FHitResult GroundHit;
			FVector EndTrace = WorldPoint;
			EndTrace.Z -= 100.f;
			FCollisionQueryParams TraceParams(FName(TEXT("GroundTrace"), true, true));
			GetWorld()->LineTraceSingleByChannel(GroundHit, WorldPoint, EndTrace, ECC_WorldStatic, TraceParams);
			if (AActor* TerrainHit = GroundHit.GetActor())
			{
				for (const TWeakObjectPtr<UPathfindingTerrain> Terrain : TerrainRegions)
				{
					if (Terrain.IsValid())
					{
						if (TerrainHit->ContainsDataLayer(Terrain.Get()->TerrainDataLayer.Get()))
						{
							MovementPenalty = Terrain.Get()->MovementPenalty;
						}
					}
				}
			}
			
			const FVector* NodeWorldPosition = new FVector(WorldPoint.X, WorldPoint.Y, WorldPoint.Z);
			FPathingNode* NewNode = new FPathingNode(bIsWalkable, *NodeWorldPosition, X, Y, MovementPenalty);
			GeneratedGrid.Add(NewNode);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Grid finished generating"));
	Grid = GeneratedGrid;
}
