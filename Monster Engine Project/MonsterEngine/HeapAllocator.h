//Class and function declarations for the HeapAllocator class

#ifndef HEAP_ALLOCATOR_H
#define HEAP_ALLOCATOR_H

#include <stdint.h>

#include "LandFillDefinitions.h"

#define ALIGN_UP(addr, align) (reinterpret_cast<uintptr_t>(addr) + (align - 1)) & ~(align - 1));
#define ALIGN_DOWN(addr, align) (reinterpret_cast<uintptr_t>(addr) & ~(align - 1));

namespace Engine {

	enum DescriptorStatus { freeDescriptor, allocatedMemory, freeMemory };

	struct BlockDescriptor
	{
		//uint8_t * pMemBlock;
		BlockDescriptor ** pMemBlock;
		size_t memBlockSize;
		DescriptorStatus status;
		BlockDescriptor * pPrevBlockDesc;
		BlockDescriptor * pNextBlockDesc;

	};

	class HeapManager {
	public:
		~HeapManager();
		void initialize(size_t i_heapSize, unsigned int i_numberOfDescriptors);
		void * alloc(size_t i_allocationSize);
		void garbageCollect();
		bool freeBlock(void * pMemBlock);
		bool shutDown();
		void printOutstandingAllocations();

	private:
		void * pHeap;
		void * pHeapEnd;
		bool shutdown = true;
		BlockDescriptor * pFreeMemory;
		BlockDescriptor * pFreeBlockDescriptors;
		BlockDescriptor * pAllocatedMemory;

		void * allocateFromBlockEnd(BlockDescriptor * i_memBlock, size_t i_size);
		void setupBlockDescriptors(unsigned int i_numberOfDescriptors);
		BlockDescriptor * getFreeDescriptor();
		size_t heapSize;

	};

}//End namespace Engine
#endif