// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathfindingGrid.generated.h"

class FPathingNode;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEADPARADISE_API UPathfindingGrid : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPathfindingGrid();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float GetNodeRadius();
	
	FPathingNode* NodeFromWorldPoint(const FVector& WorldPosition) const;

	TArray<FPathingNode*> GetNeighbouringNodes(const FPathingNode* Node);
	
private:
	friend class FPathfindingComponentVisualizer;
	
	UPROPERTY(EditAnywhere, Category = "A* Grid", meta = (AllowPrivateAccess = "true"))
	FVector2D GridWorldSize = FVector2D(1000);

	UPROPERTY(EditAnywhere, Category = "A* Grid", meta = (AllowPrivateAccess = "true"))
	float NodeRadius = 50.f;
	
	UPROPERTY(EditAnywhere, Category = "A* Grid", meta = (AllowPrivateAccess = "true"))
	UDataLayerAsset* UnwalkableDataLayer = nullptr;

	float NodeDiameter;
	int32 GridSizeX;
	int32 GridSizeY;
	FPathingNode* Grid = nullptr;
	
	UFUNCTION(CallInEditor, Category = "A* Grid", meta = (ToolTip="Generates grid data to be visualized in the editor, the data will be lost when playing or closing the editor"))
	void GenerateGrid();
};
