#pragma once
#include <algorithm>
#include <stdexcept>

template <typename T>
class DSA
{
public:
	DSA()
		:
		arr(new T[max_size])
	{}
	DSA(size_t size)
		:
		max_size(size),
		size(size),
		arr(new T[max_size])
	{}
	DSA(size_t size, const T& default_val)
		:
		DSA(size)
	{
		for (size_t i = 0; i < size; i++)
		{
			arr[i] = default_val;
		}
	}
	~DSA()
	{
		delete[] arr;
	}
	DSA(const DSA& rhs)
	{
		*this = rhs;
	}
	DSA& operator=(const DSA& rhs)
	{
		if (this == &rhs)
			return *this;
		delete arr;

		max_size = rhs.max_size;
		size = rhs.size;
		arr = new T[rhs.GetSize()];
		std::copy(rhs.arr, rhs.arr + rhs.size, arr);
		return *this;
	}

	void PushBack(const T& val)
	{
		if (size == max_size)
		{
			Resize(size * 2);
		}
		arr[size++] = val;
	}

	const T& operator[](size_t i) const
	{
		if (i < 0 || i >= size)
		{
			throw std::out_of_range("Index out of range");
		}
		return arr[i];
	}
	T& operator[](size_t i)
	{
		if (i < 0 || i >= size)
		{
			throw std::out_of_range("Index out of range");
		}
		return arr[i];
	}

	void Resize(size_t new_size)
	{
		if (new_size <= max_size)
		{
			size = new_size;
			return;
		}
		T* temp = arr;
		arr = new T[new_size];
		std::copy(temp, temp + size, arr);
		delete[] temp;
		max_size = new_size;
	}

	size_t GetSize() const
	{
		return size;
	}
	size_t GetCapacity() const
	{
		return max_size;
	}

private:
	size_t max_size = 1;
	size_t size = 0;
	T* arr = nullptr;
};