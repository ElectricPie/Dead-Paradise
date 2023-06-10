// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeapItem.h"

/**
 * 
 */
class DEADPARADISE_API FPathingNode : public THeapItem<FPathingNode>
{
public:
	FPathingNode();
	virtual ~FPathingNode() override;
	bool operator==(const FPathingNode& OtherNode) const;

	int32 GCost;
	int32 HCost;
	FPathingNode* ParentNode = nullptr;

	void SetupNode(bool IsWalkable, const FVector& NodeWorldPosition, int32 NodeGridX, int32 NodeGridY);

	bool IsWalkable() const;
	FVector GetWorldPosition() const;
	int32 GetGridX() const;
	int32 GetGridY() const;

	int32 GetFCost() const;

	using THeapItem::CompareTo;
	virtual int32 CompareTo(const FPathingNode* NodeToCompare) override;

private:
	bool bIsWalkable;
	FVector WorldPosition;
	int32 GridX;
	int32 GridY;
};
