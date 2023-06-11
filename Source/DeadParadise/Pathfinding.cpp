// Fill out your copyright notice in the Description page of Project Settings.

#include "Pathfinding.h"

#include "Heap.h"
#include "PathfindingGrid.h"
#include "PathingNode.h"

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
	PathingGridComponent = GetOwner()->GetComponentByClass<UPathfindingGrid>();
	if (!PathingGridComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Pathfinding grid not found on \"%s\""), *GetOwner()->GetActorNameOrLabel());
	}

	GetOwner()->GetWorldTimerManager().SetTimer(DebugTimerHandle, this, &UPathfinding::DebugPathFind, 1.f, false, 1.f);
}


// Called every frame
void UPathfinding::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (!Seeker) return;
	const FPathingNode* NodeAtSeeker = PathingGridComponent->NodeFromWorldPoint(Seeker->GetActorLocation());
	if (!NodeAtSeeker) return;
	DrawDebugBox(GetWorld(), NodeAtSeeker->GetWorldPosition(), FVector(PathingGridComponent->GetNodeRadius()) * 0.9f,
	             FColor::Cyan);

	if (!Target) return;
	const FPathingNode* NodeAtTarget = PathingGridComponent->NodeFromWorldPoint(Target->GetActorLocation());
	if (!NodeAtTarget) return;
	DrawDebugBox(GetWorld(), NodeAtTarget->GetWorldPosition(), FVector(PathingGridComponent->GetNodeRadius()) * 0.9f,
	             FColor::Orange);
	
	FindPath(Seeker->GetActorLocation(), Target->GetActorLocation());
}

void UPathfinding::FindPath(const FVector& StartPosition, const FVector& TargetPosition)
{
	if (!PathingGridComponent) return;

	FPathingNode* StartNode = PathingGridComponent->NodeFromWorldPoint(StartPosition);
	FPathingNode* TargetNode = PathingGridComponent->NodeFromWorldPoint(TargetPosition);

	THeap<FPathingNode> OpenSet = THeap<FPathingNode>(PathingGridComponent->GetGridSize());
	TSet<FPathingNode*> ClosedSet;

	OpenSet.Add(StartNode);

	while (OpenSet.Count() > 0)
	{	
		FPathingNode* CurrentNode = OpenSet.RemoveFirst();
		ClosedSet.Add(CurrentNode);

		if (*CurrentNode == *TargetNode)
		{
			RetracePath(StartNode, CurrentNode);
			return;
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

void UPathfinding::DebugPathFind()
{
}

void UPathfinding::RetracePath(const FPathingNode* StartNode, FPathingNode* EndNode) const
{
	TArray<FPathingNode*> Path;
	FPathingNode* CurrentNode = EndNode;

	while (CurrentNode->GetGridX() != StartNode->GetGridX() || CurrentNode->GetGridY() != StartNode->GetGridY())
	{
		Path.Add(CurrentNode);
		CurrentNode = CurrentNode->ParentNode;
		DrawDebugSphere(GetWorld(), CurrentNode->GetWorldPosition(), PathingGridComponent->GetNodeRadius(), 12,
		                FColor::Black);
	}

	Algo::Reverse(Path);
}
