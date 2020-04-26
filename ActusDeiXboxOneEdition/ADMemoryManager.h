#pragma once
#include "pch.h"
#include <stdlib.h>
#include <crtdbg.h>
#include <cstring>
#include <memory>
#include <assert.h>
using namespace std;

namespace
{
	const int ADMEMORY_ARRAY_SIZE = 1000;
	const long long FIVE_GB = 4500000000;
}

class MemoryManager
{
public:
	MemoryManager();
	~MemoryManager();
	void* Allocate(size_t size);					// Allocates a certain amount of the memoryBuffer for variable
	void Deallocate(void* object);					// Returns the memory from the deleted object back into the memoryBuffer
	size_t GetAvailableHandle();					// Returns current avaiable handle
	void* GetHandleObject(size_t index);			// Returns the object at the specified index of the handles array
private:
	char* memoryBuffer;								// Buffer that holds 5 GB of allocated memory
	long long allocatedSize;						// Variable that holds the current amount of memory being used
	char* memPointers[ADMEMORY_ARRAY_SIZE] = {};	// Array that holds the pointers to different locations in the memory buffer
	size_t memPointerIndex = 0;						// Current index of the memPointers array
	size_t sizes[ADMEMORY_ARRAY_SIZE] = {};			// Array that stores the sizes of all the objects that were allocated
	size_t sizesIndex = 0;							// Current index of the sizes array
	char* handles[ADMEMORY_ARRAY_SIZE] = {};		// Array the holds indirect paths to memory
	size_t handleIndex = 0;							// Current index of the handles array
	size_t availableHandle = 0;						// Current available handle for the Handle class

	size_t GetNextHandle();							// Helper function that returns the next available index of the handles array
};

extern MemoryManager memoryManager;

template<typename Type>
class Handle
{
public:
	Handle()
	{
		isArray = false;
		ptr = nullptr;
		handleIndex = -1;
	}

	Handle(size_t arraySize)
	{
		ptr = (Type*)memoryManager.Allocate(arraySize * sizeof(Type));
		handleIndex = memoryManager.GetAvailableHandle();
		isArray = true;
	}

	Handle(const Handle<Type>& that)
	{
		*this = that;
	}

	~Handle()
	{
		Delete();
	}

	Type* GetPointer()
	{
		ptr = (Type*)memoryManager.GetHandleObject(handleIndex);
		if (!ptr)
			assert(NULL);
		return ptr;
	}

	Type& operator[](int index)
	{
		ptr = (Type*)memoryManager.GetHandleObject(handleIndex);
		if (!ptr)
			assert(NULL);
		return ptr[index];
	}

	Type* operator->()
	{
		ptr = (Type*)memoryManager.GetHandleObject(handleIndex);
		if (!ptr)
			assert(NULL);
		return ptr;
	}

	Type& operator*()
	{
		ptr = (Type*)memoryManager.GetHandleObject(handleIndex);
		if (!ptr)
			assert(NULL);
		return *ptr;
	}

	Handle<Type>& operator=(const Handle<Type>& that)
	{
		if (this != that)
		{
			this->handleIndex = that.handleIndex;
			this->ptr = that.ptr;
			this->isArray = that.isArray;
		}
		return *this;
	}

#pragma region New()

	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	void New()
	{
		if (!isArray)
		{
			Type localVar;
			ptr = (Type*)memoryManager.Allocate(sizeof(localVar));
			memcpy(ptr, &localVar, sizeof(localVar));
			handleIndex = memoryManager.GetAvailableHandle();
		}
	}

	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	// 1 parameter version
	template <typename First>
	void New(First firstParam)
	{
		if (!isArray)
		{
			Type localVar(firstParam);
			ptr = (Type*)memoryManager.Allocate(sizeof(localVar));
			memcpy(ptr, &localVar, sizeof(localVar));
			handleIndex = memoryManager.GetAvailableHandle();
		}
	}

	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	// 2 parameter version
	template <typename First, typename Second>
	void New(First firstParam, Second secondParam)
	{
		if (!isArray)
		{
			Type localVar(firstParam, secondParam);
			ptr = (Type*)memoryManager.Allocate(sizeof(localVar));
			memcpy(ptr, &localVar, sizeof(localVar));
			handleIndex = memoryManager.GetAvailableHandle();
		}
	}

	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	// 3 parameter version
	template <typename First, typename Second, typename Third>
	void New(First firstParam, Second secondParam, Third thirdParam)
	{
		if (!isArray)
		{
			Type localVar(firstParam, secondParam, thirdParam);
			ptr = (Type*)memoryManager.Allocate(sizeof(localVar));
			memcpy(ptr, &localVar, sizeof(localVar));
			handleIndex = memoryManager.GetAvailableHandle();
		}
	}

	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	// 4 parameter version
	template <typename First, typename Second, typename Third, typename Fourth>
	void New(First firstParam, Second secondParam, Third thirdParam, Fourth fourthParam)
	{
		if (!isArray)
		{
			Type localVar(firstParam, secondParam, thirdParam, fourthParam);
			ptr = (Type*)memoryManager.Allocate(sizeof(localVar));
			memcpy(ptr, &localVar, sizeof(localVar));
			handleIndex = memoryManager.GetAvailableHandle();
		}
	}

	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	// 5 parameter version
	template <typename First, typename Second, typename Third, typename Fourth, typename Fifth>
	void New(First firstParam, Second secondParam, Third thirdParam, Fourth fourthParam, Fifth fifthParam)
	{
		if (!isArray)
		{
			Type localVar(firstParam, secondParam, thirdParam, fourthParam, fifthParam);
			ptr = (Type*)memoryManager.Allocate(sizeof(localVar));
			memcpy(ptr, &localVar, sizeof(localVar));
			handleIndex = memoryManager.GetAvailableHandle();
		}
	}

	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	// 6 parameter version
	template <typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth>
	void New(First firstParam, Second secondParam, Third thirdParam, Fourth fourthParam, Fifth fifthParam, Sixth sixthParam)
	{
		if (!isArray)
		{
			Type localVar(firstParam, secondParam, thirdParam, fourthParam, fifthParam, sixthParam);
			ptr = (Type*)memoryManager.Allocate(sizeof(localVar));
			memcpy(ptr, &localVar, sizeof(localVar));
			handleIndex = memoryManager.GetAvailableHandle();
		}
	}

	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	// 7 parameter version
	template <typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth, typename Seventh>
	void New(First firstParam, Second secondParam, Third thirdParam, Fourth fourthParam, Fifth fifthParam, Sixth sixthParam, Seventh seventhParam)
	{
		if (!isArray)
		{
			Type localVar(firstParam, secondParam, thirdParam, fourthParam, fifthParam, sixthParam, seventhParam);
			ptr = (Type*)memoryManager.Allocate(sizeof(localVar));
			memcpy(ptr, &localVar, sizeof(localVar));
			handleIndex = memoryManager.GetAvailableHandle();
		}
	}

	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	// 8 parameter version
	template <typename First, typename Second, typename Third, typename Fourth, typename Fifth, typename Sixth, typename Seventh, typename Eighth>
	void New(First firstParam, Second secondParam, Third thirdParam, Fourth fourthParam, Fifth fifthParam, Sixth sixthParam, Seventh seventhParam, Eighth eighthParam)
	{
		if (!isArray)
		{
			Type localVar(firstParam, secondParam, thirdParam, fourthParam, fifthParam, sixthParam, seventhParam, eighthParam);
			ptr = (Type*)memoryManager.Allocate(sizeof(localVar));
			memcpy(ptr, &localVar, sizeof(localVar));
			handleIndex = memoryManager.GetAvailableHandle();
		}
	}

	// Array version of New() if array constructor isn't called
	// Allocates a required chunk of memory from the memory buffer
	// If the array constructor is called this function isn't neccessary
	// CALL THIS FUNCTION BEFORE CALLING ANYTHING ELSE
	void NewArray(size_t arraySize)
	{
		if (!isArray)
		{
			ptr = (Type*)memoryManager.Allocate(arraySize * sizeof(Type));
			handleIndex = memoryManager.GetAvailableHandle();
			isArray = true;
		}
	}

#pragma endregion

	// Deallocates the previously allocated memory
	// CALL THIS FUNCTION ONLY AFTER CALLING ANY VARIATION OF New()
	void Delete()
	{
		if (handleIndex >= 0)
		{
			ptr = (Type*)memoryManager.GetHandleObject(handleIndex);
			memoryManager.Deallocate(ptr);
			ptr = nullptr;
			handleIndex = -1;
		}
	}
private:
	int handleIndex;
	Type* ptr;
	bool isArray;
};