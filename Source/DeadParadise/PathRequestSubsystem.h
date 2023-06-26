// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PathRequestSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DEADPARADISE_API UPathRequestSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
};
