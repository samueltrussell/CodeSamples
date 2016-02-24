
#ifndef STRING_POOL_H
#define STRING_POOL_H

#include <stdint.h>

namespace Engine {

	class StringPool {
	public:
		
		static StringPool * Create(size_t i_bytesInPool);

		~StringPool();

		//add a new string
		const char * add(const char * i_pString);
		//check for string in pool
		const char * find(const char * i_pString);

	private:
		StringPool(uint8_t * i_pPool, size_t i_sizePool);

		uint8_t * _pBuffer;
		uint8_t * _pEndOfStrings;
		uint8_t * _pEndBuffer;
		size_t _sizePool;

	};

}//namespace Engine


#endif
