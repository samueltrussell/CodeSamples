#include "SmartPointer.h"

#include <assert.h>

#include "DebugLog.h"

namespace Engine
{
	template <typename T>
	SmartPointer<T>::SmartPointer() :
		_pObject(NULL),
		_pRefCounter(NULL)
	{

	}

	template <typename T>
	SmartPointer<T>::SmartPointer(T * i_pObject) :
		_pObject(i_pObject),
		_pRefCounter(new long(1))
	{

	}

	template <typename T>
	SmartPointer <T>::SmartPointer(SmartPointer <T> & i_SmartPointer) :
		_pObject(i_SmartPointer._pObject),
		_pRefCounter(i_SmartPointer._pRefCounter)
	{
		(*_pRefCounter)++;
	}

	template <typename T>
	SmartPointer <T>::~SmartPointer()
	{
		ReleaseReference();
	}

	template <typename T>
	SmartPointer <T> & SmartPointer<T>::operator=(SmartPointer <T> & i_SmartPointer)
	{
		//handle self-assignment
		if (i_SmartPointer != *this)
		{
			ReleaseReference();
			AcquireReference(i_SmartPointer);
		}
		
		return *this;
	}

	template <typename T>
	T * SmartPointer <T>::operator->() 
	{
		assert(_pObject != nullptr);
		return _pObject;
	}

	template <typename T>
	T & SmartPointer<T>::operator*() 
	{
		assert(_pObject != nullptr);
		return *_pObject;
	}

	template <typename T>
	bool SmartPointer <T>::operator==(const SmartPointer <T> & i_SmartPointer) const
	{
		return (_pObject == i_SmartPointer._pObject);
	}

	template <typename T>
	bool SmartPointer <T>::operator==(const int & i_input) const
	{
		assert(i_input == NULL);
		return (_pObject == NULL);
	}

	template <typename T>
	bool SmartPointer <T>::operator!=(const SmartPointer <T> & i_SmartPointer) const
	{
		return !(_pObject == i_SmartPointer._pObject);
	}

	template <typename T>
	bool SmartPointer <T>::operator!() const
	{
		return (_pObject == NULL);
	}

	template <typename T>
	void SmartPointer<T>::AcquireReference(const SmartPointer <T> &  i_SmartPointer)
	{
		_pObject = i_SmartPointer._pObject;
		_pRefCounter = i_SmartPointer._pRefCounter;

		if(_pObject != NULL)
			(*_pRefCounter)++;
	}

	template <typename T>
	void SmartPointer<T>::ReleaseReference()
	{
		if (_pObject != NULL) {

			assert(*_pRefCounter > 0);

			if (--(*_pRefCounter) == 0)
			{
				DEBUG_LOG("Destroying Reference Counted object due to deletion of last reference.");
				delete _pRefCounter;
				delete _pObject;
			}
		}
	}

}//namespace Engine