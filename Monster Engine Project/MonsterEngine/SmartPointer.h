
#ifndef SMART_POINTER_H
#define SMART_POINTER_H

namespace Engine
{
	template <typename T>
	class SmartPointer
	{
	public:
		inline SmartPointer();
		inline SmartPointer(T * i_pObject);
		inline SmartPointer(SmartPointer <T> & i_SmartPointer);
		inline ~SmartPointer();

		//Assignment 
		inline SmartPointer <T> & operator=(SmartPointer <T> & i_SmartPointer);
		
		//MemberAccess
		inline T * operator->();
		//Indirection
		inline T & operator*();
		
		//Comparison Operators
		inline bool operator==(const SmartPointer <T> & i_SmartPointer) const;
		inline bool operator==(const int & i_input) const;
		inline bool operator!=(const SmartPointer <T> & i_SmartPointer) const;
		inline bool operator!() const;

		//LifeCycle Management Functions
		inline void AcquireReference(const SmartPointer <T> & i_SmartPointer);
		inline void ReleaseReference();

	private:
		T * _pObject;
		long * _pRefCounter;

	};

}//namespace Engine

#include "SmartPointer-inl.h"

#endif //SMART_POINTER_H

