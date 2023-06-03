// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathfindingGrid.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEADPARADISE_API UPathfindingGrid : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPathfindingGrid();

	float Test;

	void TestMethod();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	friend class FPathfindingComponentVisualizer;
	
	UPROPERTY(EditAnywhere, Category = "A* Grid", meta = (AllowPrivateAccess = "true"))
	FVector2D GridWorldSize;

	UPROPERTY(EditAnywhere, Category = "A* Grid", meta = (AllowPrivateAccess = "true"))
	float NodeRadius;

	float NodeDiameter;
	FVector2D GridSize;
		
};
