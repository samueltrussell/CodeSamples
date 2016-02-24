//Function definitons for the HeapAllocator class

#include "HeapAllocator.h"

#include <stdint.h>
#include <stdlib.h>

#include "ConsolePrint.h"
#include "LandFillDefinitions.h"
#include "MessagedAssert.h"

#ifdef _WIN64
	#define PtrFmt "0x%016x"
#else
	#define PtrFmt "0x%08x"
#endif

namespace Engine {

	HeapManager::~HeapManager()
	{
		shutDown();
	}

	void HeapManager::initialize(size_t i_heapSize, unsigned int i_numberOfDescriptors)
	{
		void * pMemBlock = malloc(i_heapSize);

		//int test = sizeof(BlockDescriptor);

		MessagedAssert(pMemBlock != NULL, "System Memory Allocation Failure!");
		MessagedAssert(i_numberOfDescriptors, "Must have at least one descriptor");
		MessagedAssert((i_numberOfDescriptors * sizeof(BlockDescriptor) < i_heapSize), "Descriptors Consume More than the"
			" Allotted Memory!");

		pHeap = pMemBlock;
		pHeapEnd = reinterpret_cast<uint8_t *>(pHeap) + i_heapSize;

		pFreeMemory = static_cast<BlockDescriptor *>(pHeap);

		//Set Up first Free Block Descriptor at head of Memory Space
		uint8_t * temp = reinterpret_cast<uint8_t *>(pFreeMemory) + i_numberOfDescriptors * sizeof(BlockDescriptor);
		pFreeMemory->pMemBlock = reinterpret_cast<BlockDescriptor **>(temp);
		*pFreeMemory->pMemBlock = pFreeMemory;
		pFreeMemory->memBlockSize = i_heapSize - i_numberOfDescriptors * sizeof(BlockDescriptor);
		pFreeMemory->pPrevBlockDesc = NULL;
		pFreeMemory->pNextBlockDesc = NULL;
		pFreeMemory->status = freeMemory;

		//init heapSize
		heapSize = pFreeMemory->memBlockSize - sizeof(BlockDescriptor **);

		//point the Free Block Descriptor list at the first free block descriptor
		pFreeBlockDescriptors = pFreeMemory + 1;

		//Set up the Free Block Descriptors and build the free block descriptor list
		setupBlockDescriptors(i_numberOfDescriptors);

		//Init the allocated memory Block Descriptor list to be empty
		pAllocatedMemory = NULL;

		shutdown = false;
	}//End initialize

	void HeapManager::setupBlockDescriptors(unsigned int i_numberOfDescriptors)
	{
		BlockDescriptor * currentBlockDescriptor = pFreeBlockDescriptors;
		BlockDescriptor * previousBlockDescriptor = NULL;

		for (unsigned int i = 0; i < (i_numberOfDescriptors - 1); i++) {
			currentBlockDescriptor->memBlockSize = 0;
			currentBlockDescriptor->pMemBlock = NULL;
			currentBlockDescriptor->pPrevBlockDesc = previousBlockDescriptor;
			if (i == i_numberOfDescriptors - 2) {
				currentBlockDescriptor->pNextBlockDesc = NULL;
			}
			else {
				currentBlockDescriptor->pNextBlockDesc = currentBlockDescriptor + 1;
			}
			currentBlockDescriptor->status = freeDescriptor;

			//iterate
			previousBlockDescriptor = currentBlockDescriptor;
			currentBlockDescriptor++;
		}

	}//End setupBlockDescriptors()

	void * HeapManager::alloc(size_t i_size) {
		bool searching = true;
		BlockDescriptor * pCurrentBlockDescriptor = pFreeMemory;
		size_t padding = sizeof(BlockDescriptor**); //add enough memory to the request to hold a pointer back to the BlockDesc

		MessagedAssert((i_size + padding < heapSize), "This heap manager cannot suppport an allocation of that size. Enlarge Heap.");

		//Search the free memory structure for the first block that fits. Ordering the list at freeBlock() optimizes space
		//by allocating from the smallest block that will hold the allocation
		while (searching && pCurrentBlockDescriptor->pNextBlockDesc != NULL) {

			if (pCurrentBlockDescriptor->memBlockSize < (i_size + padding)) { //Nailed it!
				pCurrentBlockDescriptor = pCurrentBlockDescriptor->pNextBlockDesc;
				continue;
			}

			searching = false;
		}

		MessagedAssert((pCurrentBlockDescriptor->memBlockSize >= i_size + padding), "Heap Manager Out of Memory! \n"
			"No available memory block can support the allocation");

		if (pCurrentBlockDescriptor->memBlockSize < i_size + padding)
			return NULL;

		return allocateFromBlockEnd(pCurrentBlockDescriptor, i_size + padding);
	}//End alloc()

	void * HeapManager::allocateFromBlockEnd(BlockDescriptor * i_memBlockFrom, size_t i_size)
	{
		//cast the BlockDescriptor ** down so that pointer math is in bytes, not pointers
		uint8_t * pFromBlockMemorySpace = reinterpret_cast<uint8_t *>(i_memBlockFrom->pMemBlock);
//		uint8_t * temp; //keep this around for pointer math

		//Move the pointer back from the end of the block far enough to hold the whole allocation
		uint8_t * pNewBlockMemorySpace = pFromBlockMemorySpace + i_memBlockFrom->memBlockSize - i_size;
		BlockDescriptor ** pNewBlock = reinterpret_cast<BlockDescriptor **>(pNewBlockMemorySpace);

		pNewBlock = reinterpret_cast<BlockDescriptor **>ALIGN_DOWN(pNewBlock, 4); //ensure 4-byte alignment of the new block

		MessagedAssert((pNewBlock >= i_memBlockFrom->pMemBlock), "After alignment, block is not sufficiently large"
			" for requested allocation");

		if (pNewBlock < i_memBlockFrom->pMemBlock)
			return NULL;

		BlockDescriptor * pNewBlockDescriptor = getFreeDescriptor();
		MessagedAssert((pNewBlockDescriptor != NULL), "Failed to acquire free block descriptor");

		if (pNewBlockDescriptor == NULL)
			return NULL;

		pNewBlockMemorySpace = reinterpret_cast<uint8_t *>(pNewBlock);

		//Set the size of the new block based on any expansions that were necessary
		pNewBlockDescriptor->pMemBlock = pNewBlock;
		pNewBlockDescriptor->memBlockSize = (pFromBlockMemorySpace + i_memBlockFrom->memBlockSize) - pNewBlockMemorySpace;

		//retain a pointer from the Memory block to the BlockDescriptor. Store it at the head of the memory block
		*pNewBlockDescriptor->pMemBlock = pNewBlockDescriptor;

		//reduce the size of the original block by the amount required for the allocation
		i_memBlockFrom->memBlockSize -= pNewBlockDescriptor->memBlockSize;

		//Fill newly allocated memory block with newAlloc Fill
		uint8_t * startFill = reinterpret_cast<uint8_t*>(pNewBlockDescriptor->pMemBlock + 1);
		uint8_t * endFill = startFill + i_size - sizeof(BlockDescriptor **);
		for (uint8_t * i = startFill; i < endFill; i++)
			*i = _bNewAllocLandFill;

		//Fill alignment padding space with AlignLandFill if necessary
		if (pNewBlockDescriptor->memBlockSize != i_size) {
			startFill = reinterpret_cast<uint8_t *>(pNewBlockDescriptor->pMemBlock) + i_size;
			endFill = reinterpret_cast<uint8_t *>(pNewBlockDescriptor->pMemBlock) + pNewBlockDescriptor->memBlockSize;
			for (uint8_t * i = startFill; i < endFill; i++)
				*i = _bAlignLandFill;
		}

		//Mark the new Descriptor as Allocated
		pNewBlockDescriptor->status = allocatedMemory;

		return (pNewBlockDescriptor->pMemBlock + 1); //return a pointer to the space beyond the BlockDescriptor pointer
	}//End AllocateFromBlockEnd()

	BlockDescriptor * HeapManager::getFreeDescriptor() {
		MessagedAssert((pFreeBlockDescriptors != NULL), "Out of free block descriptors! Make more!");
		if (pFreeBlockDescriptors == NULL)
			return NULL;

		//pop new descriptor from the top of the FreeDescriptor stack
		BlockDescriptor * pNewBlockDescriptor = pFreeBlockDescriptors;
		if (pNewBlockDescriptor->pNextBlockDesc != NULL) //as long as this isn't the last one in the list
			pNewBlockDescriptor->pNextBlockDesc->pPrevBlockDesc = NULL;
		pFreeBlockDescriptors = pNewBlockDescriptor->pNextBlockDesc;

		//push new descriptor onto the top of the AllocatedMemory stack
		pNewBlockDescriptor->pNextBlockDesc = pAllocatedMemory;
		if (pAllocatedMemory != NULL) // as long as the list isn't empty, link the current head list item back to the new descriptor
			pAllocatedMemory->pPrevBlockDesc = pNewBlockDescriptor;
		pNewBlockDescriptor->pPrevBlockDesc = NULL;
		pAllocatedMemory = pNewBlockDescriptor;

		return pNewBlockDescriptor;
	}//End getFreeDescriptor

	bool HeapManager::freeBlock(void * i_pMemBlock)
	{
		MessagedAssert(i_pMemBlock != NULL, "Invalid attempt to free a NULL pointer");
		if (i_pMemBlock == NULL)
			return NULL;

		//Back-linked memory implementation: Descriptor<->Memory
		//move the pointer back before the BlockDesc pointer
		BlockDescriptor ** temp = reinterpret_cast<BlockDescriptor **>(i_pMemBlock);
		BlockDescriptor * pBlockDescriptor = *(temp - 1);

		MessagedAssert((pAllocatedMemory != NULL), "It appears that this heap manager has no allocated Memory blocks to free.");

		MessagedAssert((pBlockDescriptor->status != freeMemory), "The Memory you are attempting to free has already been freed");

		MessagedAssert((pBlockDescriptor->status != freeDescriptor), "The Memory you are attemping to free points to a Descriptor"
			" that doesn't have a record of being allocated");

		MessagedAssert((pBlockDescriptor->status == allocatedMemory), "The Memory you are attempting to free has what appears to be"
			" a bad reference to a BlockDescriptor");

		MessagedAssert((pBlockDescriptor->pMemBlock == temp - 1), "The block descriptor referenced by the memory you are"
			" attempting to free does not point back to that memory. This is an indication of a heap manager bug, or that the"
			" pointer you sent points to memory that was already freed by another process");

		if (pBlockDescriptor->status != allocatedMemory ||
			pAllocatedMemory == NULL ||
			pBlockDescriptor->pMemBlock != temp - 1)
			return false;

		//We found the right descriptor, remove it from the AllocatedMemory structure

		if (pBlockDescriptor->pPrevBlockDesc == NULL) //the descriptor to remove is at the head of the list
			pAllocatedMemory = pBlockDescriptor->pNextBlockDesc;
		else
			pBlockDescriptor->pPrevBlockDesc->pNextBlockDesc = pBlockDescriptor->pNextBlockDesc;

		if (pBlockDescriptor->pNextBlockDesc != NULL) //the descriptor to remove is not at the end of the list
			pBlockDescriptor->pNextBlockDesc->pPrevBlockDesc = pBlockDescriptor->pPrevBlockDesc;

		//push the freed descriptor onto the FreeMemory list===

		//find the first value in the list bigger than this one
		BlockDescriptor * pCurrentBlockDesc = pFreeMemory;
		while (pCurrentBlockDesc->memBlockSize < pBlockDescriptor->memBlockSize &&
			pCurrentBlockDesc->pNextBlockDesc != NULL) {

			pCurrentBlockDesc = pCurrentBlockDesc->pNextBlockDesc;
		}

		//grab linked list values off the bigger block
		pBlockDescriptor->pNextBlockDesc = pCurrentBlockDesc;
		pBlockDescriptor->pPrevBlockDesc = pCurrentBlockDesc->pPrevBlockDesc;

		//Set the linked list values of the neighbors;
		pCurrentBlockDesc->pPrevBlockDesc = pBlockDescriptor;
		//Only try to update the previous block if we're not at the head of the list
		if (pBlockDescriptor->pPrevBlockDesc != NULL)
			pBlockDescriptor->pPrevBlockDesc->pNextBlockDesc = pBlockDescriptor;
		if (pBlockDescriptor->pPrevBlockDesc == NULL)
			pFreeMemory = pBlockDescriptor;

		//Set the block descriptor to freeMemory
		pBlockDescriptor->status = freeMemory;

		//fill freed memory with freed landfill===
		uint8_t * startFill = reinterpret_cast<uint8_t *>(pBlockDescriptor->pMemBlock + 1);
		uint8_t * endFill = reinterpret_cast<uint8_t *>(pBlockDescriptor->pMemBlock) + pBlockDescriptor->memBlockSize;

		for (uint8_t * i = startFill; i < endFill; i++)
			*i = _bFreedLandFill;

		return true; //successful De-Allocation
	}//end freeBlock()

	void HeapManager::garbageCollect()
	{
		//run through the list of free memory blocks
		BlockDescriptor * pCurrentBlockDescriptor = pFreeMemory;
		BlockDescriptor * pNextBlockDescriptor;
		uint8_t * pNextBlock;

		while (pCurrentBlockDescriptor != NULL) {
			//examine the memory space next to the first free descriptor
			pNextBlock = reinterpret_cast<uint8_t *>(pCurrentBlockDescriptor->pMemBlock) + pCurrentBlockDescriptor->memBlockSize;

			if (pNextBlock >= pHeapEnd) {
				pCurrentBlockDescriptor = pCurrentBlockDescriptor->pNextBlockDesc;
				continue;
			}

			pNextBlockDescriptor = *reinterpret_cast<BlockDescriptor **>(pNextBlock);

			if (pNextBlockDescriptor->status == freeMemory) { //if the subsequent block is a freed block

				pCurrentBlockDescriptor->memBlockSize += pNextBlockDescriptor->memBlockSize; //expand to consume the next block

				//relink the free memory list around the consumed block
				if (pNextBlockDescriptor->pNextBlockDesc != NULL) //beware the last one in the list
					pNextBlockDescriptor->pNextBlockDesc->pPrevBlockDesc = pNextBlockDescriptor->pPrevBlockDesc;
				if (pNextBlockDescriptor->pPrevBlockDesc != NULL) //beware the head of the list
					pNextBlockDescriptor->pPrevBlockDesc->pNextBlockDesc = pNextBlockDescriptor->pNextBlockDesc;
				if (pNextBlockDescriptor->pPrevBlockDesc == NULL)
					pFreeMemory = pNextBlockDescriptor->pNextBlockDesc;

				//free the block descriptor
				pNextBlockDescriptor->memBlockSize = NULL;
				pNextBlockDescriptor->pMemBlock = NULL;
				pNextBlockDescriptor->status = freeDescriptor;

				//add it to the head of the free descriptor list
				pNextBlockDescriptor->pPrevBlockDesc = NULL;
				pNextBlockDescriptor->pNextBlockDesc = pFreeBlockDescriptors;
				pFreeBlockDescriptors = pNextBlockDescriptor;

				//Write over the BlockDescriptor* with free memory 
				for (int i = 0; i < 4; i++)
					*pNextBlock++ = _bFreedLandFill;


				//continue, checking the block beyond the end of the block we just merged
				continue;
			}
			else { //the next block in memory is not free, don't merge it. Move on down the freeMemory list
				pCurrentBlockDescriptor = pCurrentBlockDescriptor->pNextBlockDesc;
			}

		}//End while loop over freeMemory list
	}//End GarbageCollect()

	bool HeapManager::shutDown()
	{
		if (shutdown)
			return false;

		DEBUG_PRINT("Shutting down Heap Allocator, initial size: %i\n", heapSize);

		if (pAllocatedMemory != NULL)
			DEBUG_PRINT("MEMORY LEAKS DETECTED IN HEAP ALLOCATOR!!!\n");

		printOutstandingAllocations();

		shutdown = true;
		free(pHeap);
		return true;
	}//End shutDown()

	void HeapManager::printOutstandingAllocations()
	{
		if (pAllocatedMemory != NULL)
		{
			DEBUG_PRINT("Outstanding Allocations in Heap Allocator, initial size: %i \n", heapSize);

			BlockDescriptor * currentDescriptor = pAllocatedMemory;
			do {
				DEBUG_PRINT("Outstanding allocation at address: " PtrFmt ", size: %i\n", currentDescriptor->pMemBlock, currentDescriptor->memBlockSize);
				currentDescriptor = currentDescriptor->pNextBlockDesc;
			} while (currentDescriptor != NULL);

		}
		else
			DEBUG_PRINT("No outstanding allocations detected\n");



	}//End printOutstandingAllocations()
}//End namespace Engine