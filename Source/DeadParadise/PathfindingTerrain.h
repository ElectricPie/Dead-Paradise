// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PathfindingTerrain.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class DEADPARADISE_API UPathfindingTerrain : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TWeakObjectPtr<UDataLayerAsset> TerrainDataLayer;

	UPROPERTY(EditAnywhere)
	int32 MovementPenalty;
};
