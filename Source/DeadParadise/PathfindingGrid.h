// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathfindingGrid.generated.h"

class FPathingNode;

UCLASS()
class DEADPARADISE_API APathfindingGrid : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APathfindingGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/**
	 * @brief Returns the radius of the grids nodes
	 * @return A float of the grids nodes size
	 */
	float GetNodeRadius() const;
	/**
	 * @brief Gets the number of nodes in the grid
	 * @return An int with the number of nodes in the grid
	 */
	int GetGridSize() const;
	/**
	 * @brief Attempts to get the node with the closest world position to the WorldPosition provided
	 * @param WorldPosition A FVector of the 
	 * @param OutNode A FPathing Node pointer to store the output
	 * @return Returns true if a node is found otherwise will return false
	 */
	bool NodeFromWorldPoint(const FVector& WorldPosition, OUT FPathingNode*& OutNode) const;
	/**
	 * @brief Attempts to get the node surrounding the provided node 
	 * @param Node The node to check its neighbours
	 * @param OutNeighboringNodes A Array to store the neighboring nodes
	 * @return Returns false if something went wrong other wise will return true
	 */
	bool GetNeighbouringNodes(const FPathingNode& Node, OUT TArray<FPathingNode*>& OutNeighboringNodes);
	
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
	TArray<FPathingNode*> Grid;
	
	UFUNCTION(CallInEditor, Category = "A* Grid", meta = (ToolTip="Generates grid data to be visualized in the editor, the data will be lost when playing or closing the editor"))
	void GenerateGrid();
};
