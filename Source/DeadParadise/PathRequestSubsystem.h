// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PathRequestSubsystem.generated.h"

DECLARE_DELEGATE_TwoParams(FOnPathRequestFinishedSignature, TArray<const FVector*> /* Path */, bool /* PathWasFound */)

class UPathfinding;
struct FPathRequest;

/**
 * 
 */
UCLASS()
class DEADPARADISE_API UPathRequestSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void RequestPath(const FVector& PathStart, const FVector& PathEnd, FOnPathRequestFinishedSignature& Callback);
	void FinishedProcessingNext(TArray<const FVector*> Path, bool bWasSuccessful);
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	void SetPathfinding(UPathfinding* PathfindingComponent);

	
private:
	UPROPERTY()
	UPathfinding* Pathfinding = nullptr;

	TQueue<FPathRequest*> PathRequestQueue;
	FPathRequest* CurrentPathRequest = nullptr;
	bool bIsProcessingPath = false;

	void TryProcessNext();
};

struct FPathRequest
{
	FVector PathStart;
	FVector PathEnd;
	FOnPathRequestFinishedSignature& Callback;

	FPathRequest(const FVector& Start, const FVector& End, FOnPathRequestFinishedSignature& FinishCallback);
};
