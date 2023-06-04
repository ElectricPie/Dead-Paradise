// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingComponentVisualizer.h"
#include "DeadParadiseEditor.h"
#include "DeadParadise/PathfindingGrid.h"

void FPathfindingComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View,
                                                        FPrimitiveDrawInterface* PDI)
{
	if (const UPathfindingGrid* Grid = Cast<UPathfindingGrid>(Component))
	{
		// Draws the box for the GridWorldSize
		DrawWorldGrid(PDI, Grid->GetOwner()->GetActorLocation(), Grid->GridWorldSize, Grid->NodeRadius * 2);
	}
}

void FPathfindingComponentVisualizer::DrawWorldGrid(FPrimitiveDrawInterface* PDI, const FVector& Location, const FVector2D Area, float NodeRadius) const
{
	// Draws the box for the GridWorldSize
	const FVector GridSize = FVector(Area, NodeRadius);
	const FBox GridArea = FBox::BuildAABB(Location, GridSize / 2);
		
	DrawWireBox(PDI, GridArea, FLinearColor::Red, SDPG_Foreground);
}
