// Fill out your copyright notice in the Description page of Project Settings.

#include "Pathfinding.h"

#include "Heap.h"
#include "PathfindingGrid.h"
#include "PathingNode.h"
#include "PathRequestSubsystem.h"

// Sets default values for this component's properties
UPathfinding::UPathfinding()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPathfinding::BeginPlay()
{
	Super::BeginPlay();

	// ...
	PathingGridComponent = Cast<APathfindingGrid>(GetOwner());
	if (!PathingGridComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Pathfinding grid not found on \"%s\""), *GetOwner()->GetActorNameOrLabel());
	}

	PathRequestSubsystem = GetWorld()->GetSubsystem<UPathRequestSubsystem>();
	PathRequestSubsystem->SetPathfinding(this);
}


// Called every frame
void UPathfinding::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPathfinding::StartFindPath(const FVector& StartPosition, const FVector& TargetPosition)
{
	// TODO: Call as coroutine
	FindPath(StartPosition, TargetPosition);
}

void UPathfinding::FindPath(const FVector& StartPosition, const FVector& TargetPosition)
{
	if (!PathingGridComponent) return;

	TArray<const FVector*> Waypoints;
	bool bPathSuccess = false;

	FPathingNode* StartNode = nullptr;
	FPathingNode* TargetNode = nullptr;

	if (!PathingGridComponent->NodeFromWorldPoint(StartPosition, StartNode) ||
		!PathingGridComponent->NodeFromWorldPoint(TargetPosition, TargetNode))
	{
		return;
	}
	
	THeap<FPathingNode> OpenSet = THeap<FPathingNode>(PathingGridComponent->GetGridSize());
	TSet<FPathingNode*> ClosedSet;

	OpenSet.Add(StartNode);

	while (OpenSet.Count() > 0)
	{	
		FPathingNode* CurrentNode = OpenSet.RemoveFirst();
		ClosedSet.Add(CurrentNode);

		if (*CurrentNode == *TargetNode)
		{
			bPathSuccess = true;
			break;
		}

		TArray<FPathingNode*> Neighbours = PathingGridComponent->GetNeighbouringNodes(CurrentNode);
		for (int32 I = 0; I < Neighbours.Num(); I++)
		{
			if (!Neighbours[I]->IsWalkable() || ClosedSet.Contains(Neighbours[I]))
			{
				continue;
			}

			const int32 NewMovementCostToNeighbour = CurrentNode->GCost + GetDistance(CurrentNode, Neighbours[I]);
			if (NewMovementCostToNeighbour < Neighbours[I]->GCost || !OpenSet.Contains(Neighbours[I]))
			{
				Neighbours[I]->GCost = NewMovementCostToNeighbour;
				Neighbours[I]->HCost = GetDistance(Neighbours[I], TargetNode);
				Neighbours[I]->ParentNode = CurrentNode;

				if (!OpenSet.Contains(Neighbours[I]))
				{
					OpenSet.Add(Neighbours[I]);
				}
			}
		}
	}
	
	// TODO: Yield return null (wait 1 frame)
	if (bPathSuccess)
	{
		Waypoints = RetracePath(StartNode, TargetNode);
	}
	PathRequestSubsystem->FinishedProcessingNext(Waypoints, bPathSuccess);
}


int32 UPathfinding::GetDistance(const FPathingNode* NodeA, const FPathingNode* NodeB)
{
	const int32 DistX = FMath::Abs(NodeA->GetGridX() - NodeB->GetGridX());
	const int32 DistY = FMath::Abs(NodeA->GetGridY() - NodeB->GetGridY());

	if (DistX > DistY)
	{
		return 14 * DistY + 10 * (DistX - DistY);
	}

	return 14 * DistX + 10 * (DistY - DistX);
}


TArray<const FVector*> UPathfinding::RetracePath(const FPathingNode* StartNode, FPathingNode* EndNode) const
{
	TArray<FPathingNode*> Path;
	FPathingNode* CurrentNode = EndNode;

	while (CurrentNode->GetGridX() != StartNode->GetGridX() || CurrentNode->GetGridY() != StartNode->GetGridY())
	{
		Path.Add(CurrentNode);
		CurrentNode = CurrentNode->ParentNode;
		DrawDebugSphere(GetWorld(), CurrentNode->GetWorldPosition(), PathingGridComponent->GetNodeRadius(), 12,
		                FColor::Black, false, 10.f);
	}

	TArray<const FVector*> Waypoints = SimplifyPath(Path);
	Algo::Reverse(Waypoints);
	return Waypoints;
}

TArray<const FVector*> UPathfinding::SimplifyPath(TArray<FPathingNode*> Path) const
{
	TArray<const FVector*> Waypoints;
	FVector2d* DirectionOld = new FVector2d(0.f);

	TArray<FVector> Test;
	
	for (int32 i = 1; i < Path.Num(); i++)
	{
		FVector2d* DirectionNew = new FVector2d(Path[i-1]->GetGridX() - Path[i]->GetGridX(),Path[i-1]->GetGridY() - Path[i]->GetGridY());
		if (DirectionNew != DirectionOld)
		{
			FVector NewWaypoint = Path[i]->GetWorldPosition();
			Waypoints.Add(&NewWaypoint);
			Test.Add(NewWaypoint);
			DirectionOld = DirectionNew;
		}
	}

	return Waypoints;
}

