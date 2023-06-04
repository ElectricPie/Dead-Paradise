// Fill out your copyright notice in the Description page of Project Settings.


#include "PathingNode.h"

PathingNode::PathingNode()
{
	bIsWalkable = false;
	WorldPosition = FVector::Zero();
}

PathingNode::~PathingNode()
{
}

void PathingNode::SetupNode(bool IsWalkable, FVector NodeWorldPosition)
{
	bIsWalkable = IsWalkable;
	WorldPosition = NodeWorldPosition;
}

FVector PathingNode::GetWorldPosition() const
{
	return WorldPosition;
}
