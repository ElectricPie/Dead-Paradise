// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Pathfinding.generated.h"

class UPathRequestSubsystem;
class APathfindingGrid;
class FPathingNode;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEADPARADISE_API UPathfinding : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPathfinding();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief Finds a path between the given world positions
	 * @param StartPosition The start of the path
	 * @param TargetPosition The end point of the path
	 */
	void StartFindPath(const FVector& StartPosition, const FVector& TargetPosition);

private:
	UPROPERTY(EditAnywhere, Category = "Test", meta = (AllowPrivateAccess = "true"))
	AActor* Seeker;
	
	UPROPERTY(EditAnywhere, Category = "Test", meta = (AllowPrivateAccess = "true"))
	AActor* Target;

	UPROPERTY()
	APathfindingGrid* PathingGridComponent = nullptr;
	UPROPERTY()
	UPathRequestSubsystem* PathRequestSubsystem;

	void FindPath(const FVector& StartPosition, const FVector& TargetPosition);
	TArray<const FVector*> RetracePath(const FPathingNode& StartNode, const FPathingNode& EndNode) const;
	
	static int32 GetDistance(const FPathingNode& NodeA, const FPathingNode& NodeB);
	static TArray<const FVector*> SimplifyPath(const TArray<const FPathingNode*>& Path);
};
