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

int32 APathfindingGrid::GetPenaltyMin() const
{
	return PenaltyMin;
}

int32 APathfindingGrid::GetPenaltyMax() const
{
	return PenaltyMax;
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
						if (TerrainHit->ContainsDataLayer(Terrain->TerrainDataLayer))
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
	BlurPenaltyMap(3);
}

void APathfindingGrid::BlurPenaltyMap(const int32 BlurSize)
{
	const int32 KernelSize = BlurSize * 2 + 1;
	const int32 KernelExtents = (KernelSize - 1) / 2;

	TArray<int32> PenaltiesHorizontalPass;
	PenaltiesHorizontalPass.Init(0, GridSizeX * GridSizeY);
	TArray<int32> PenaltiesVerticalPass;
	PenaltiesVerticalPass.Init(0, GridSizeX * GridSizeY);

	// Horizontal pass
	for (int32 y = 0; y < GridSizeY; y++)
	{
		for (int32 x = -KernelExtents; x <= KernelExtents; x++)
		{
			const int32 SampleX = FMath::Clamp(x, 0, KernelExtents);
			PenaltiesHorizontalPass[0 * GridSizeY + y] += Grid[SampleX * GridSizeY + y]->MovementPenalty;
		}

		for (int32 x = 1; x < GridSizeX; x++)
		{
			const int32 RemoveIndex = FMath::Clamp(x - KernelExtents - 1, 0, GridSizeX);
			const int32 AddIndex = FMath::Clamp(x + KernelExtents, 0, GridSizeX - 1);

			PenaltiesHorizontalPass[x * GridSizeY + y] = PenaltiesHorizontalPass[(x - 1) * GridSizeY + y]
			- Grid[RemoveIndex * GridSizeY + y]->MovementPenalty
			+ Grid[AddIndex * GridSizeY + y]->MovementPenalty;
		}
	}

	// Vertical pass
	for (int32 x = 0; x < GridSizeX; x++)
	{
		for (int32 y = -KernelExtents; y <= KernelExtents; y++)
		{
			const int32 SampleY = FMath::Clamp(y, 0, KernelExtents);
			PenaltiesVerticalPass[x * GridSizeY + 0] += PenaltiesHorizontalPass[x * GridSizeY + SampleY];
		}
		
		int32 BlurredPenalty = FMath::RoundToInt32(static_cast<float>(PenaltiesVerticalPass[x * GridSizeY + 0]) / (KernelSize * KernelSize));
		Grid[x * GridSizeY + 0]->MovementPenalty = BlurredPenalty;

		for (int32 y = 1; y < GridSizeX; y++)
		{
			const int32 RemoveIndex = FMath::Clamp(y - KernelExtents - 1, 0, GridSizeY);
			const int32 AddIndex = FMath::Clamp(y + KernelExtents, 0, GridSizeY - 1);

			PenaltiesVerticalPass[x * GridSizeY + y] = PenaltiesVerticalPass[x * GridSizeY + y - 1]
			- PenaltiesHorizontalPass[x * GridSizeY + RemoveIndex]
			+ PenaltiesHorizontalPass[x * GridSizeY + AddIndex];

			BlurredPenalty = FMath::RoundToInt32(static_cast<float>(PenaltiesVerticalPass[x * GridSizeY + y]) / (KernelSize * KernelSize));
			Grid[x * GridSizeY + y]->MovementPenalty = BlurredPenalty;

			if (BlurredPenalty > PenaltyMax)
			{
				PenaltyMax = BlurredPenalty;
			}

			if (BlurredPenalty < PenaltyMin)
			{
				PenaltyMin = BlurredPenalty;
			}
		}
	}
}
