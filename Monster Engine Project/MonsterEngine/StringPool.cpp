//Function Definitions for class StringPool

#include "StringPool.h"

#include <cstring>

#include "MessagedAssert.h"

namespace Engine {

	StringPool::StringPool(uint8_t * i_pBuffer, size_t i_sizePool) :
		_pBuffer(i_pBuffer),
		_pEndOfStrings(i_pBuffer),
		_pEndBuffer(i_pBuffer + i_sizePool),
		_sizePool(i_sizePool)
	{

	}

	StringPool::~StringPool() {
		
		delete _pBuffer;
	}

	StringPool * StringPool::Create(size_t i_bytesInPool) 
	{

		uint8_t * pPool = new uint8_t[i_bytesInPool];

		return new StringPool(pPool, i_bytesInPool);
	}

	const char * StringPool::add(const char * i_string) 
	{
		//get string size, include null terminator
		size_t stringSize = std::strlen(i_string) + 1;

		//check if the string is already in the pool
		const char * pPooledString = find(i_string);

		if (pPooledString == NULL) {
			
			//Ensure there is enough room in the pool to add the new string
			MessagedAssert(((_pEndOfStrings + stringSize + 1) < _pEndBuffer), "StringPool out of Memory. Increase size of String Pool");

			//insert string size
			*(size_t * )_pEndOfStrings = stringSize;

			uint8_t * pNewString = _pEndOfStrings + sizeof(stringSize);

			//insert string contents
			for (size_t i = 0; i < stringSize; i++) {
				pNewString[i] = i_string[i];
			}

			//advance pointer to end of new string
			_pEndOfStrings = pNewString + stringSize;

			//return pointer to newly added string
			return reinterpret_cast<char *>(pNewString);

		}

		return pPooledString;
	}

	const char * StringPool::find(const char * i_string) 
	{

		//get string size for increased comparison speed, include null terminator
		size_t stringSize = strlen(i_string) + 1;

		//loop through the pool searching for the string
		uint8_t * iterator = _pBuffer;

		while (iterator < _pEndOfStrings) {

			size_t pooledStringSize = *reinterpret_cast<size_t*>(iterator);

			if (pooledStringSize == stringSize) {

				char * pPooledString = reinterpret_cast<char*>(iterator + sizeof(pooledStringSize));
				bool equivalent = true;

				//Peform bytewise comparison of strings
				for (size_t i = 0; i < stringSize; i++) {

					//if any two bytes are not equal, exit the comparison
					if (pPooledString[i] != i_string[i]) {
						equivalent = false;
						break;
					}
				}

				if (equivalent) {
					return pPooledString;
				}

			}

			iterator += sizeof(pooledStringSize) + pooledStringSize;				
		}

		return NULL;
	}


}//namespace Engine