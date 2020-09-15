#pragma once


namespace Util
{
	/**
	* Releases a COM object and nullifies pointer
	* 
	* @param it is the interface to release
	*/
	template<typename InterfaceType>
	inline void Release(InterfaceType** it)
	{
		if (*it)
		{
			(*it)->Release();
			*it = nullptr;
		}
	}

	/**
	* Acquires an additional reference, if non-null
	* 
	* @param it is the interface to add a reference to
	* @returns the interface
	*/
	template<typename InterfaceType>
	inline InterfaceType* Aquire(InterfaceType* it)
	{
		if (it)
			it->AddRef();

		return it;
	}

	/**
	* Sets a new COM object, acquiring the reference
	* 
	* @param currentObj is the object which will be the newObj
	* @param newObj is the new interface
	*/
	template<typename InterfaceType>
	inline void Attach(InterfaceType** currentObj, InterfaceType* newObj)
	{
		Util::Release(currentObj);
		*currentObj = newObj;
	}

	/**
	* Releases a COM object and nullifies pointer
	* 
	* @param it is the interface
	* @returns the new old interface
	*/
	template<typename InterfaceType>
	inline InterfaceType* Detach(InterfaceType** it)
	{
		InterfaceType* oldObj = *it;
		*it = nullptr;
		return oldObj;
	}
}


