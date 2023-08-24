// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitPathfinding.h"

#include "PathfindingGrid.h"
#include "PathRequestSubsystem.h"
#include "Unit.h"

// Sets default values for this component's properties
UUnitPathfinding::UUnitPathfinding()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;

	// ...
}

void UUnitPathfinding::InitializeComponent()
{
	Super::InitializeComponent();
	Unit = Cast<AUnit>(GetOwner());
	if (!Unit)
	{
		UE_LOG(LogTemp, Error, TEXT("UnitPathfinding component not on an Unit : On %s"), *GetOwner()->GetActorNameOrLabel());
	}
}


// Called when the game starts
void UUnitPathfinding::BeginPlay()
{
	Super::BeginPlay();

	// ...
	PathRequest = GetWorld()->GetSubsystem<UPathRequestSubsystem>();
}


// Called every frame
void UUnitPathfinding::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	DrawRemainingPath();
}

void UUnitPathfinding::OnPathFound(TArray<const FVector*> NewPath, bool bPathWasFound)
{
	if (!bPathWasFound || !Unit) return;
	
	Path = NewPath;

	CurrentWaypointIndex = 0;
	
	Unit->MoveToPoint(*Path[CurrentWaypointIndex], this, FName("OnMoveFinished"));
}

void UUnitPathfinding::DrawRemainingPath()
{
	if (Path.IsEmpty()) return;

	int32 NodeSize = 50.f;
	if (PathfindingGrid)
	{
		NodeSize = PathfindingGrid->GetNodeRadius();
	}
	
	DrawDebugLine(GetWorld(), Unit->GetActorLocation(), *Path[CurrentWaypointIndex], FColor::Red,
		false, FApp::GetDeltaTime());
	
	for (int32 i = CurrentWaypointIndex; i < Path.Num(); i ++)
	{
		DrawDebugSphere(GetWorld(), *Path[i], NodeSize, 12,
						FColor::Red, false, FApp::GetDeltaTime());

		if (i + 1 < Path.Num())
		{
			DrawDebugLine(GetWorld(), *Path[i], *Path[i + 1], FColor::Red, false,
				FApp::GetDeltaTime());
		}
	}
}

void UUnitPathfinding::OnMoveFinished(bool bReachedTarget)
{
	if (CurrentWaypointIndex++ >= Path.Num() - 1)
	{
		Path.Empty();
		return;
	}
	
	Unit->MoveToPoint(*Path[CurrentWaypointIndex], this, FName("OnMoveFinished"));
}

void UUnitPathfinding::PathfindToPosition(const FVector& WorldPosition)
{
	FOnPathRequestFinishedSignature Callback = FOnPathRequestFinishedSignature::CreateUObject(this, &UUnitPathfinding::OnPathFound);
	PathRequest->RequestPath(GetOwner()->GetActorLocation(), WorldPosition, Callback);
}
