// Fill out your copyright notice in the Description page of Project Settings.


#include "PathingNode.h"

FPathingNode::FPathingNode(const bool IsWalkable, const FVector& NodeWorldPosition, const int32 NodeGridX, const int32 NodeGridY) : WorldPosition(NodeWorldPosition)
{
	bIsWalkable = IsWalkable;
	GridX = NodeGridX;
	GridY = NodeGridY;
	GCost = -1;
	HCost = -1;
}

FPathingNode::~FPathingNode()
{
}

bool FPathingNode::operator==(const FPathingNode& OtherNode) const
{
	return GridX == OtherNode.GridX && GridY == OtherNode.GridY && WorldPosition == OtherNode.WorldPosition;
}

const FVector& FPathingNode::GetWorldPosition() const
{
	return WorldPosition;
}

int32 FPathingNode::GetGridX() const
{
	return GridX;
}

int32 FPathingNode::GetGridY() const
{
	return GridY;
}


bool FPathingNode::IsWalkable() const
{
	return bIsWalkable;
}

int FPathingNode::GetFCost() const
{
	return GCost + HCost;
}

int32 FPathingNode::CompareTo(const FPathingNode& NodeToCompare)
{
	int32 Compare = FMath::Clamp(GetFCost() - NodeToCompare.GetFCost(), -1, 1);
	if (Compare == 0)
	{
		Compare = FMath::Clamp(HCost - NodeToCompare.HCost, -1, 1);
	}
	return -Compare;
}
