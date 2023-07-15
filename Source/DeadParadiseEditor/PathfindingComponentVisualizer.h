// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"

class APathfindingGrid;

/**
 * 
 */
class DEADPARADISEEDITOR_API FPathfindingComponentVisualizer : public FComponentVisualizer
{
public:
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View,
		FPrimitiveDrawInterface* PDI) override;
	
	void DrawWorldGrid(FPrimitiveDrawInterface* PDI, const FVector& Location, FVector2D Area, float NodeRadius) const;

	void DrawGridNodes(FPrimitiveDrawInterface* PDI, const APathfindingGrid* PathfindingGrid) const;
};
 