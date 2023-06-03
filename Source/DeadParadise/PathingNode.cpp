// Fill out your copyright notice in the Description page of Project Settings.


#include "PathingNode.h"

PathingNode::PathingNode(bool NodeIsWalkable, FVector* NodeWorldPosition)
{
	bIsWalkable = NodeIsWalkable;
	WorldPosition = NodeWorldPosition;
}

PathingNode::~PathingNode()
{
}
