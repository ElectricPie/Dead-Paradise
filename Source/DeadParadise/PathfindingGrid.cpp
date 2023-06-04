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
	if (!TestActor) return;

	const PathingNode* NodeAtActor = NodeFromWorldPoint(TestActor->GetActorLocation());
	if (!NodeAtActor) return;
	
	// UE_LOG(LogTemp, Warning, TEXT("Actor Node World Pos: %s"), *NodeAtActor->GetWorldPosition().ToString());
	
	DrawDebugBox(GetWorld(), NodeAtActor->GetWorldPosition(), FVector(NodeRadius) * 0.9f, FColor::Cyan);
}

PathingNode* UPathfindingGrid::NodeFromWorldPoint(const FVector& WorldPosition) const
{
	if (Grid == nullptr) return nullptr;
	
	float PosX = (WorldPosition.X - GetOwner()->GetActorLocation().X + GridWorldSize.X * 0.5f) / NodeDiameter;
	float PoxY = (WorldPosition.Y - GetOwner()->GetActorLocation().Y + GridWorldSize.Y * 0.5f) / NodeDiameter;
	
	PosX = FMath::Clamp(PosX, 0, GridWorldSize.X - 1);
	PoxY = FMath::Clamp(PoxY, 0, GridWorldSize.Y - 1);

	const int NodeArrayX = FMath::FloorToInt(PosX);
	const int NodeArrayY = FMath::FloorToInt(PoxY);

	return &Grid[NodeArrayX*GridSizeY+NodeArrayY];
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
			
			Grid[X*GridSizeY+Y].SetupNode(bIsWalkable, WorldPoint);
		}
	}
}
