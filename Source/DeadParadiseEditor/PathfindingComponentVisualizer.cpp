// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingComponentVisualizer.h"

#include "DeadParadiseEditor.h"
#include "DeadParadise/PathfindingGrid.h"
#include "DeadParadise/PathfindingGridVisualiser.h"
#include "DeadParadise/PathingNode.h"
#include "Kismet/KismetMathLibrary.h"


void FPathfindingComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View,
                                                        FPrimitiveDrawInterface* PDI)
{
	if (const UPathfindingGridVisualiser* GridVisualiser = Cast<UPathfindingGridVisualiser>(Component))
	{
		if (const APathfindingGrid* PathfindingGrid = GridVisualiser->PathfindingGrid) {
			// Draws the box for the GridWorldSize
			DrawWorldGrid(PDI,
				PathfindingGrid->GetActorLocation(),
				PathfindingGrid->GridWorldSize,
				PathfindingGrid->NodeRadius * 2);

			DrawGridNodes(PDI, *PathfindingGrid);
		}
	}
}

void FPathfindingComponentVisualizer::DrawWorldGrid(FPrimitiveDrawInterface* PDI, const FVector& Location, const FVector2D Area, float NodeRadius) const
{
	// Draws the box for the GridWorldSize
	const FVector GridSize = FVector(Area, NodeRadius);
	const FBox GridArea = FBox::BuildAABB(Location, GridSize / 2);
		
	DrawWireBox(PDI, GridArea, FLinearColor::Red, SDPG_Foreground);
}


void FPathfindingComponentVisualizer::DrawGridNodes(FPrimitiveDrawInterface* PDI, const APathfindingGrid& PathfindingGrid) const
{
	if (PathfindingGrid.Grid.IsEmpty()) return;

	const int32 MinPenalty = PathfindingGrid.PenaltyMin;
	const int32 MaxPenalty = PathfindingGrid.PenaltyMax;

	const TArray<FPathingNode*> Grid = PathfindingGrid.Grid;
	
	// Draws the box for each node
	for (int X = 0; X < PathfindingGrid.GridSizeX; X++)
	{
		for (int Y = 0; Y < PathfindingGrid.GridSizeY; Y++)
		{
			const FPathingNode* Node = Grid[X * PathfindingGrid.GridSizeY + Y];
			FLinearColor NodeColor = UnwalkableColor;
			if (Node->IsWalkable())
			{
				NodeColor = UKismetMathLibrary::LinearColorLerp(MinPenaltyColor, MaxPenaltyColor, UKismetMathLibrary::NormalizeToRange(Node->MovementPenalty, MinPenalty, MaxPenalty));
			}
			const FVector NodeSize = FVector(PathfindingGrid.NodeRadius) * 0.9f;
			const FBox NodeBox = FBox::BuildAABB(Node->GetWorldPosition(), NodeSize * 0.9f);
					
			DrawWireBox(PDI, NodeBox, NodeColor, SDPG_World);
		}
	}
}
