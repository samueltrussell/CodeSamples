//Function Declarations for overriding the new, delete, new[], and delete operators
#pragma once

#include "FixedBlockAllocator.h"
#include "HeapAllocator.h"

// standard new / delete
void * operator new(size_t i_size);
void operator delete(void * i_ptr);

//Specialized HeapManager new / delete
void * operator new(size_t i_size, Engine::HeapManager * pHeap, int garbage);
void operator delete(void * i_ptr, Engine::HeapManager * pHeap);

//Specialized FixedBlockAllocator new / delete
void * operator new(size_t i_size, Engine::FixedBlockAllocator * pAllocator);
void operator delete(void * i_ptr, Engine::FixedBlockAllocator * pAllocator);

// standard array new / delete
void * operator new[](size_t i_size);
void operator delete[](void * i_ptr);

// standard placement array new / delete
void * operator new(size_t i_size, void * i_pmem);
void operator delete(void * i_ptr, void * i_pmem);

// standard array placement array new / delete
void * operator new[](size_t i_size, void * i_pmem);
void operator delete[](void * i_ptr, void * i_pmem);
