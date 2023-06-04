// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class DEADPARADISE_API PathingNode
{
public:
	PathingNode();
	~PathingNode();

	void SetupNode(bool IsWalkable, FVector NodeWorldPosition);

	FVector GetWorldPosition() const;

	bool IsWalkable() const;

private:
	bool bIsWalkable;
	FVector WorldPosition;
};
