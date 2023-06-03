// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingComponentVisualizer.h"
#include "DeadParadiseEditor.h"
#include "DeadParadise/PathfindingGrid.h"

void FPathfindingComponentVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View,
                                                        FPrimitiveDrawInterface* PDI)
{
	if (const UPathfindingGrid* Grid = Cast<UPathfindingGrid>(Component))
	{
		UE_LOG(DeadParadiseEditor, Warning, TEXT("Drawing Visualization"));
		
		// DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 12, FColor::Red, false);
		DrawWireSphere(PDI, Grid->GetOwner()->GetActorLocation(), FColor::Red, 10.f, 12, SDPG_Foreground);
	}
}
