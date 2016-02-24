//Function definitions for the FixedBlockAllocator class

#include "FixedBlockAllocator.h"

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

namespace Engine
{

	FixedBlockAllocator::~FixedBlockAllocator()
	{
		shutDown();
	}

	bool FixedBlockAllocator::initialize()
	{
		//Get heap allocated by the System
		int heapSize = numBlocks * blockSize;

		//Initialize pointers to head and tail of heap
		pMemory = reinterpret_cast<uint8_t *>(malloc(heapSize));
		pLast = pMemory + heapSize;

		//fill the whole space with freed landfill
		for (uint8_t * i = pMemory; i < pLast; i++)
			*i = _bFreedLandFill;

		//Run throught the heap and initialize hidden pointers 
		for (uint8_t * i = pMemory; i < pLast - blockSize; i += blockSize)
		{
			*reinterpret_cast<uint8_t **>(i) = i + blockSize;
		}

		*reinterpret_cast<uint8_t **>(pLast - blockSize) = NULL;

		pFreeMemory = pMemory;

		shutdown = false;
		return true;
	}

	void * FixedBlockAllocator::alloc()
	{
		MessagedAssert(pFreeMemory != NULL, "Fixed Block Allocator is out of available memory blocks!");
		if (pFreeMemory == NULL)
			return NULL;

		//get the first block off the top of the free list
		uint8_t * pBlock = pFreeMemory;

		//repoint the free list to the next block
		pFreeMemory = *reinterpret_cast<uint8_t **>(pFreeMemory);

		//fill the newly allocated block with "allocated" landfill
		for (uint8_t * i = pBlock; i < pBlock + blockSize; i++)
		{
			*i = _bNewAllocLandFill;
		}
		
		return pBlock;
	}

	bool FixedBlockAllocator::freeBlock(void * i_blockToFree)
	{
		//Make sure this is a block from this heap
		MessagedAssert((pMemory <= i_blockToFree && i_blockToFree <= pLast - blockSize), "Invalid atttempt to free a block not allocated by this Fixed Block Allocator");
		if (i_blockToFree < pMemory || pLast < i_blockToFree)
			return false;

		//Make sure this is a pointer to an actual block, not somewhere in the middle of a block
		MessagedAssert(((reinterpret_cast<uint8_t *>(i_blockToFree) - pMemory) % blockSize == 0), "Invalid attempt to free a pointer that is in the heap, but does not point to the head of a block");
		if ((reinterpret_cast<uint8_t *>(i_blockToFree) - pMemory) % blockSize != 0)
			return false;

		//Make sure this is not a pointer to a block that has already been freed
		uint8_t * test = reinterpret_cast<uint8_t *>(i_blockToFree) + sizeof(void *);
		MessagedAssert((*test != _bFreedLandFill), "Invalid attempt to free a memory block that has already been freed!");
		if (*test == _bFreedLandFill)
			return false;

		//Fill the returned memory with "freed" landfill
		for (uint8_t * i = reinterpret_cast<uint8_t *>(i_blockToFree); i < reinterpret_cast<uint8_t *>(i_blockToFree) + blockSize; i++)
			*i = _bFreedLandFill;

		//Add the hidden pointer to the next free block
		*reinterpret_cast<uint8_t **>(i_blockToFree) = pFreeMemory;

		//repoint the FreeMemory list at the newly freed block
		pFreeMemory = reinterpret_cast<uint8_t *>(i_blockToFree);

		return true;
	}//End freeBlock()

	bool FixedBlockAllocator::shutDown()
	{
		//Don't run the shutdown if it has already been done
		if (shutdown)
			return false;

		bool outstandingAllocations = false;

		DEBUG_PRINT("Shutting Down fixed block allocator, block size: %i, numBlocks: %i\n", blockSize, numBlocks);
		
		//Search Sequentially through the memory space for any hidden pointers not pointing at valid, free blocks
		for (uint8_t * i = pMemory; i <= pLast - blockSize; i += blockSize)
		{
			uint8_t ** test = reinterpret_cast<uint8_t **>(i);
			
			//Check for the case of the last free block on the free list
			if (*test == NULL)
			{
				test++;
				if (*reinterpret_cast<uint8_t *>(test) != _bFreedLandFill)
				{
					outstandingAllocations = true;
					DEBUG_PRINT("Outstanding allocation at address: " PtrFmt ", size: %i\n", i, blockSize);
				}
				continue;
			}

			//make sure its in the valid memory space
			if(*test < pMemory || pLast < *test)
			{
				outstandingAllocations = true;
				DEBUG_PRINT("Outstanding allocation at address: " PtrFmt ", size: %i\n", i, blockSize);
				continue;
			}

			//make sure it points at a valid block
			if ((reinterpret_cast<uint8_t *>(*test) - pMemory) % blockSize != 0)
			{
				outstandingAllocations = true;
				DEBUG_PRINT("Outstanding allocation at address: " PtrFmt ", size: %i\n", i, blockSize);
				continue;
			}

		}

		if (!outstandingAllocations)
			DEBUG_PRINT("No Outstanding Allocations\n");
		else
			DEBUG_PRINT("MEMORY LEAKS DETECTED IN FIXED BLOCK ALLOCATOR, BLOCKSIZE: %i\n", blockSize);

		//Deallocate Memory back to System
		free(pMemory);
		shutdown = true;

		return outstandingAllocations;
	}//End shutDown()

}//End namespace Engine