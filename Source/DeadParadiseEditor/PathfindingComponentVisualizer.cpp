// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingComponentVisualizer.h"
#include "DeadParadiseEditor.h"
#include "DeadParadise/PathfindingGrid.h"
#include "DeadParadise/PathingNode.h"

void FPathfindingComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View,
                                                        FPrimitiveDrawInterface* PDI)
{
	if (const UPathfindingGrid* PathfindingGrid = Cast<UPathfindingGrid>(Component))
	{
		// Draws the box for the GridWorldSize
		DrawWorldGrid(PDI,
			PathfindingGrid->GetOwner()->GetActorLocation(),
			PathfindingGrid->GridWorldSize,
			PathfindingGrid->NodeRadius * 2);

		DrawGridNodes(PDI, PathfindingGrid);
	}
}

void FPathfindingComponentVisualizer::DrawWorldGrid(FPrimitiveDrawInterface* PDI, const FVector& Location, const FVector2D Area, float NodeRadius) const
{
	// Draws the box for the GridWorldSize
	const FVector GridSize = FVector(Area, NodeRadius);
	const FBox GridArea = FBox::BuildAABB(Location, GridSize / 2);
		
	DrawWireBox(PDI, GridArea, FLinearColor::Red, SDPG_Foreground);
}

void FPathfindingComponentVisualizer::DrawGridNodes(FPrimitiveDrawInterface* PDI, const UPathfindingGrid* PathfindingGrid) const
{
	if (PathfindingGrid->Grid != nullptr)
	{
		for (int X = 0; X < PathfindingGrid->GridSizeX; X++)
		{
			for (int Y = 0; Y < PathfindingGrid->GridSizeY; Y++)
			{
				// Draws the box for each node
				const PathingNode* Grid = PathfindingGrid->Grid;
				FLinearColor WalkableColor = Grid[X*PathfindingGrid->GridSizeY+Y].IsWalkable() ? FLinearColor::Green : FLinearColor::Red;
				const FVector NodeSize = FVector(PathfindingGrid->NodeRadius) * 0.9f;
				const FBox NodeBox = FBox::BuildAABB(Grid[X*PathfindingGrid->GridSizeY+Y].GetWorldPosition(), NodeSize * 0.9f);
					
				DrawWireBox(PDI, NodeBox, WalkableColor, SDPG_World);
			}
		}
	}
}
