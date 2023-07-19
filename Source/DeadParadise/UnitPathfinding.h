// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UnitPathfinding.generated.h"


class UPathRequestSubsystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEADPARADISE_API UUnitPathfinding : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUnitPathfinding();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	UPathRequestSubsystem* PathRequest = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "Debug", meta=(AllowPrivateAccess = "true"))
	AActor* Target;
	
	TArray<const FVector*> Path;

	void OnPathFound(TArray<const FVector*> NewPath, bool bPathWasFound);
	void FollowPath();

	UFUNCTION()
	void DebugPathfinding();
};
