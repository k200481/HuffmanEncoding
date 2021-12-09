#pragma once
#include "DSA.h"

template <typename T>
class Heap
{
public:
	Heap() = default;
	Heap(size_t size)
		:
		arr(size)
	{}
	Heap(bool (*cmp)(const T&, const T&))
		:
		cmp(cmp)
	{}
	Heap(size_t size, bool (*cmp)(const T&, const T&))
		:
		arr(size),
		cmp(cmp)
	{}

	void Push(const T& val)
	{
		if (idx == arr.GetSize())
		{
			arr.PushBack(val);
			idx++;
			ShiftUp(idx - 1);
			return;
		}

		arr[idx++] = val;
		ShiftUp(idx - 1);
	}
	const T& Peek() const
	{
		if (idx == 0)
			throw std::exception("Heap is Empty");
		return arr[0];
	}
	void Pop()
	{
		if (idx == 0)
			throw std::exception("Heap is Empty");
		else if (idx == 1)
		{
			idx--;
			return;
		}

		idx--;
		std::swap(arr[0], arr[idx]);
		Heapify(0);
	}

	bool IsEmpty() const
	{
		return idx == 0;
	}
	size_t GetSize()
	{
		return idx;
	}

private:
	void Heapify(size_t i)
	{
		size_t l = i * i + 1;
		size_t r = i * i + 2;
		size_t p = i;
		
		if (l < idx && cmp(arr[l], arr[p]))
			p = l;
		if (r < idx && cmp(arr[r], arr[p]))
			p = r;

		if (i != p)
		{
			std::swap(arr[i], arr[p]);
			Heapify(p);
		}
	}
	void ShiftUp(size_t i)
	{
		size_t p;
		if (i % 2 == 1)
		{
			// left child
			p = (i - 1) / 2;
		}
		if (i % 2 == 0)
		{
			// right child
			p = (i - 2) / 2;
		}
		
		if (p < i && cmp(arr[i], arr[p]))
		{
			std::swap(arr[i], arr[p]);
			ShiftUp(p);
		}
	}

private:
	DSA<T> arr;
	size_t idx = 0;
	bool (*cmp)(const T&, const T&) = [](const T& l, const T& r) { return l > r; };
};