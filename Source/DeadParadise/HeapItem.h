// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
template <typename T>
class DEADPARADISE_API THeapItem
{
public:
	virtual ~THeapItem() = default;
	int32 HeapIndex = -1;

	virtual int32 CompareTo(const T* ItemToCompare) = 0;
};