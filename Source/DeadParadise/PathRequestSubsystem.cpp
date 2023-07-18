// Fill out your copyright notice in the Description page of Project Settings.


#include "PathRequestSubsystem.h"

#include "Pathfinding.h"


void UPathRequestSubsystem::RequestPath(const FVector& PathStart, const FVector& PathEnd,
	FOnPathRequestFinishedSignature& Callback)
{
	if (!Pathfinding)
	{
		UE_LOG(LogTemp, Error, TEXT("PathfindingSubsystem has no referance to UPathfinding in RequestPath method"));
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Boop"));
	FPathRequest* NewRequest = new FPathRequest(PathStart, PathEnd, Callback);
	PathRequestQueue.Enqueue(NewRequest);

	TryProcessNext();
}

void UPathRequestSubsystem::FinishedProcessingNext(TArray<const FVector*> Path, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Finished Processing"));
	CurrentPathRequest->Callback.Execute(Path, bWasSuccessful);
	bIsProcessingPath = false;
	TryProcessNext();
}

void UPathRequestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Display, TEXT("Pathfinding Request System Running"));
}

void UPathRequestSubsystem::SetPathfinding(UPathfinding* PathfindingComponent)
{
	if (Pathfinding)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pathfinding has already been set on %s"), *Pathfinding->GetOwner()->GetName());
		return;
	}

	Pathfinding = PathfindingComponent;
}

void UPathRequestSubsystem::TryProcessNext()
{
	if (!bIsProcessingPath && !PathRequestQueue.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Processing Next"));
		PathRequestQueue.Dequeue(CurrentPathRequest);
		bIsProcessingPath = true;
		Pathfinding->StartFindPath(CurrentPathRequest->PathStart, CurrentPathRequest->PathEnd);
	}
}

FPathRequest::FPathRequest(const FVector& Start, const FVector& End, FOnPathRequestFinishedSignature& FinishCallback) : Callback(FinishCallback)
{
	PathStart = Start;
	PathEnd = End;
}
