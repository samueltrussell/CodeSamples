//Function Declarations for overriding the new, delete, new[], and delete operators

#include "Allocators.h"

#include <malloc.h>

#include "ConsolePrint.h"

#ifdef _WIN64
	#define PtrFmt "0x%016x"
#else
	#define PtrFmt "0x%08x"
#endif

void * operator new(size_t i_size)
{
	void * ptr = _aligned_malloc(i_size, 4);
	
	DEBUG_PRINT("Calling operator new(size_t) with size: %i, returning address: " PtrFmt " \n", i_size, ptr);
	
	return ptr;
}//End operator new(i_size)

void operator delete(void * i_ptr)
{
	DEBUG_PRINT("Calling operator delete(void *) on address: " PtrFmt " \n", i_ptr);

	_aligned_free(i_ptr);

}//End operator delete(void *)

void * operator new(size_t i_size, Engine::HeapManager * i_pHeap, int garbage)
{
	void * ptr = i_pHeap->alloc(i_size);
	
	DEBUG_PRINT("Calling operator new(size_t, Engine::HeapManger *) with size: %i, returning address: " PtrFmt " \n", i_size, ptr);

	return ptr;
}

void operator delete(void * i_ptr, Engine::HeapManager * i_pHeap)
{
	DEBUG_PRINT("Calling operator delete(void *, Engine::HeapManager *) on address: " PtrFmt " \n", i_ptr);

	i_pHeap->freeBlock(i_ptr);
}

void * operator new(size_t i_size, Engine::FixedBlockAllocator * i_pHeap)
{
	void * ptr = i_pHeap->alloc();

DEBUG_PRINT("Calling operator new(size_t, Engine::FixedBlockAllocator *) for object of size: %i, returning address: " PtrFmt " \n", i_size, ptr);

return ptr;
}

void operator delete(void * i_ptr, Engine::FixedBlockAllocator * i_pHeap)
{
	DEBUG_PRINT("Calling operator delete(void *, Engine::FixedBlockAllocator *) on address: " PtrFmt " \n", i_ptr);

	i_pHeap->freeBlock(i_ptr);
}