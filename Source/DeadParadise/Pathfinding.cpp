// Fill out your copyright notice in the Description page of Project Settings.

#pragma PRAGMA_DISABLE_OPTIMIZATION

#include "Pathfinding.h"

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
}


// Called every frame
void UPathfinding::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (!Seeker) return;

	const FPathingNode* NodeAtSeeker = PathingGridComponent->NodeFromWorldPoint(Seeker->GetActorLocation());
	if (!NodeAtSeeker) return;

	DrawDebugBox(GetWorld(), NodeAtSeeker->GetWorldPosition(), FVector(PathingGridComponent->GetNodeRadius()) * 0.9f, FColor::Cyan);

	if (!Target) return;

	const FPathingNode* NodeAtTarget = PathingGridComponent->NodeFromWorldPoint(Target->GetActorLocation());
	if (!NodeAtTarget) return;

	DrawDebugBox(GetWorld(), NodeAtTarget->GetWorldPosition(), FVector(PathingGridComponent->GetNodeRadius()) * 0.9f, FColor::Orange);

	FindPath(Seeker->GetActorLocation(), Target->GetActorLocation());
}

void UPathfinding::FindPath(const FVector& StartPosition, const FVector& TargetPosition)
{
	if (!PathingGridComponent) return;
	
	FPathingNode* StartNode = PathingGridComponent->NodeFromWorldPoint(StartPosition);
	FPathingNode* TargetNode = PathingGridComponent->NodeFromWorldPoint(TargetPosition);
	
	TArray<FPathingNode*> OpenSet;
	TSet<FPathingNode*> ClosedSet;
	
	OpenSet.Init(StartNode, 1);
	
	while (OpenSet.Num() > 0)
	{
		FPathingNode* CurrentNode = OpenSet[0];
		for (int32 I = 1; I < OpenSet.Num(); I++)
		{
			if (OpenSet[I]->GetFCost() < CurrentNode->GetFCost() ||
				OpenSet[I]->GetFCost() == CurrentNode->GetFCost())
			{
				if (OpenSet[I]->HCost < CurrentNode->HCost)
				{
					CurrentNode = OpenSet[I];
				}
			}
		}

		OpenSet.Remove(CurrentNode);
		ClosedSet.Add(CurrentNode);

		if (CurrentNode->GetGridX() == TargetNode->GetGridX() && CurrentNode->GetGridY() == TargetNode->GetGridY())
		{
			RetracePath(StartNode, CurrentNode);
			return;
		}
		
		for (FPathingNode* Neighbour : PathingGridComponent->GetNeighbouringNodes(CurrentNode))
		{
			if (!Neighbour->IsWalkable() || ClosedSet.Contains(Neighbour))
			{
				continue;
			}
		
			const int32 NewMovementCostToNeighbour = CurrentNode->GCost + GetDistance(CurrentNode, Neighbour);
			if (NewMovementCostToNeighbour < Neighbour->GCost || !OpenSet.Contains(Neighbour))
			{
				Neighbour->GCost = NewMovementCostToNeighbour;
				Neighbour->HCost = GetDistance(Neighbour, TargetNode);
				Neighbour->ParentNode = new FPathingNode();
				*Neighbour->ParentNode = *CurrentNode;
			
				if (!OpenSet.Contains(Neighbour))
				{
					OpenSet.Add(Neighbour);
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

void UPathfinding::RetracePath(const FPathingNode* StartNode, FPathingNode* EndNode) const
{
	TArray<FPathingNode*> Path;
	FPathingNode* CurrentNode = EndNode;

	while (CurrentNode->GetGridX() != StartNode->GetGridX() || CurrentNode->GetGridY() != StartNode->GetGridY())
	{
		Path.Add(CurrentNode);
		CurrentNode = CurrentNode->ParentNode;
		DrawDebugSphere(GetWorld(), CurrentNode->GetWorldPosition(), PathingGridComponent->GetNodeRadius(), 12, FColor::Black);		
	}

	Algo::Reverse(Path);
}
