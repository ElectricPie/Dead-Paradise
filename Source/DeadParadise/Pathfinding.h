// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Pathfinding.generated.h"

class UPathfindingGrid;
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

	void FindPath(const FVector& StartPosition, const FVector& TargetPosition);

	int32 GetDistance(const FPathingNode* NodeA, const FPathingNode* NodeB);
	
private:
	UPROPERTY(EditAnywhere, Category = "Test", meta = (AllowPrivateAccess = "true"))
	AActor* Seeker;

	UPROPERTY(EditAnywhere, Category = "Test", meta = (AllowPrivateAccess = "true"))
	AActor* Target;
	
	UPathfindingGrid* PathingGridComponent = nullptr;

	void RetracePath(const FPathingNode* StartNode, FPathingNode* EndNode) const;
};
