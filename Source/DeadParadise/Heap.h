// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Heap.h"
#include "HeapItem.h"

class FHeapItem;

/**
 * 
 */
template<class T = FHeapItem>
class DEADPARADISE_API THeap
{
	static_assert(TIsDerivedFrom<T, THeapItem<T>>::IsDerived, "Type must inherit from FHeapItem");
	
public:
	explicit THeap(int32 MaxHeapSize);
	~THeap();

	int32 Count() const;
	void Add(T& Item);
	T& RemoveFirst();
	bool Contains(T& Item);
	void UpdateItem(const T& Item);

private:
	T** Items;
	int32 CurrentItemCount;
	
	void SortUp(T& Item);
	void SwapEl(T& ItemA, T& ItemB);
	void SortDown(T& Item);
};


template <typename T>
THeap<T>::THeap(const int32 MaxHeapSize)
{
	Items = new T*[MaxHeapSize];
	CurrentItemCount = 0;
}

template <typename T>
THeap<T>::~THeap()
{
}

template <typename T>
int32 THeap<T>::Count() const
{
	return CurrentItemCount;
}

template <typename T>
void THeap<T>::Add(T& Item)
{
	Item.HeapIndex = CurrentItemCount;
	Items[CurrentItemCount] = &Item;
	SortUp(*Items[CurrentItemCount]);
	CurrentItemCount++;
}

template <typename T>
T& THeap<T>::RemoveFirst()
{
	T& FirstItem = *Items[0];
	CurrentItemCount--;
	Items[0] = Items[CurrentItemCount];
	Items[0]->HeapIndex = 0;
	SortDown(*Items[0]);
	return FirstItem;
}

template <typename T>
bool THeap<T>::Contains(T& Item)
{
	if (T* ItemAtHeapIndex = Items[Item.HeapIndex])
	{
		return &Item == ItemAtHeapIndex;
	}
	
	return false;
}

template <typename T>
void THeap<T>::UpdateItem(const T& Item)
{
	SortUp(Item);
}

template <typename T>
void THeap<T>::SortUp(T& Item)
{
	int32 ParentIndex = (Item.HeapIndex - 1) / 2;
	while (true)
	{
		T& ParentItem = *Items[ParentIndex];
		const int32 CompareValue = Item.CompareTo(ParentItem);
		if (CompareValue > 0)
		{
			SwapEl(Item, ParentItem);
		}
		else
		{
			break;
		}

		ParentIndex = (Item.HeapIndex - 1) / 2;
	}
	
}

template <typename T>
void THeap<T>::SwapEl(T& ItemA, T& ItemB)
{
	Swap(Items[ItemA.HeapIndex], Items[ItemB.HeapIndex]);
	const int ItemAIndex = ItemA.HeapIndex;
	ItemA.HeapIndex = ItemB.HeapIndex;
	ItemB.HeapIndex = ItemAIndex;
	
}

template <typename T>
void THeap<T>::SortDown(T& Item)
{
	while (true)
	{
		int32 ChildIndexLeft = Item.HeapIndex * 2 + 1;
		int32 ChildIndexRight = Item.HeapIndex * 2 + 2;
		int32 SwapIndex = 0;

		if (ChildIndexLeft < CurrentItemCount)
		{
			SwapIndex = ChildIndexLeft;

			if (ChildIndexRight < CurrentItemCount)
			{
				if (Items[ChildIndexLeft]->CompareTo(*Items[ChildIndexRight]) < 0)
				{
					SwapIndex = ChildIndexRight;
				}
			}

			if (Item.CompareTo(*Items[SwapIndex]) < 0)
			{
				SwapEl(Item, *Items[SwapIndex]);
			}
			else
			{
				return;
			}
		}
		else
		{
			return;
		}
	}
}
