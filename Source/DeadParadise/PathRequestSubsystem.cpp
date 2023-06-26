// Fill out your copyright notice in the Description page of Project Settings.


#include "PathRequestSubsystem.h"


void UPathRequestSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Display, TEXT("Pathfinding Request System Running"));
}
