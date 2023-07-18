// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitPathfinding.h"

#include "PathRequestSubsystem.h"

// Sets default values for this component's properties
UUnitPathfinding::UUnitPathfinding()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUnitPathfinding::BeginPlay()
{
	Super::BeginPlay();

	// ...
	PathRequest = GetWorld()->GetSubsystem<UPathRequestSubsystem>();
	
	FOnPathRequestFinishedSignature Callback = FOnPathRequestFinishedSignature::CreateUObject(this, &UUnitPathfinding::OnPathFound);
	PathRequest->RequestPath(GetOwner()->GetActorLocation(), Target->GetActorLocation(), Callback);
}


// Called every frame
void UUnitPathfinding::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UUnitPathfinding::OnPathFound(TArray<const FVector*> NewPath, bool bPathWasFound)
{
	if (!bPathWasFound) return;
	
	Path = NewPath;

	for (int32 i = 0; i < Path.Num(); i ++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Waypoint %d: %s"), i, *NewPath[i]->ToString());
		
		DrawDebugSphere(GetWorld(), *NewPath[i], 50.f, 12,
						FColor::Red, false, 10.f);
	}
	// Use timer
}

// TODO: Handled movement in the Unit
void UUnitPathfinding::FollowPath()
{
	const FVector* CurrentWaypoint = Path[0];
}

