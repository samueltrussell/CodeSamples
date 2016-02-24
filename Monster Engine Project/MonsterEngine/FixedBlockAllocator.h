//Class and function declarations for the FixedBlockAllocator class

#ifndef FIXED_BLOCK_ALLOCATOR_H
#define FIXED_BLOCK_ALLOCATOR_H

#include <stdint.h>
#include <stdlib.h>

namespace Engine
{

	class FixedBlockAllocator
	{
	public:
		FixedBlockAllocator(int i_blockSize, int i_numBlocks) : blockSize(i_blockSize), numBlocks(i_numBlocks) {};
		~FixedBlockAllocator();
		bool initialize();
		void * alloc();
		bool freeBlock(void * i_blockToFree);
		bool shutDown();

	private:
		int blockSize;
		int numBlocks;
		bool shutdown = true;
		uint8_t * pMemory;
		uint8_t * pFreeMemory;
		uint8_t * pLast;

	};

}//End Namespace Engine
#endif //FIXED_BLOCK_ALLOCATOR_H