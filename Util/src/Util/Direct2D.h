#pragma once

namespace DWrite
{
	/**
	* QUESTION:
	*	Should I do this?
	*	Should I use "typedef" or "using"
	*/

	typedef DWRITE_FONT_WEIGHT FontWeight;
	typedef DWRITE_FONT_STYLE FontStyle;
	typedef DWRITE_FONT_STRETCH FontStretch;
	typedef DWRITE_MEASURING_MODE MeasuringMode;

	typedef DWRITE_TEXT_METRICS TextMetrics;
	typedef DWRITE_LINE_METRICS LineMetrics;
	typedef DWRITE_CLUSTER_METRICS ClusterMetrics;
	typedef DWRITE_OVERHANG_METRICS OverhangMetrics;
	typedef DWRITE_HIT_TEST_METRICS HitTestMetrics;

	typedef DWRITE_TEXT_RANGE TextRange;
}

namespace Direct2D
{
	typedef D2D1_RECT_F RectF;
	typedef D2D1_DRAW_TEXT_OPTIONS DrawTextOptions;
}



namespace Util
{
	/// <summary>
	/// Releases a COM object and nullifies pointer
	/// </summary>
	/// <typeparam name="InterfaceType">Is any type which inherits from IUnknown</typeparam>
	/// <param name="obj">is the interface to release</param>
	template<typename InterfaceType, typename = std::enable_if_t<std::is_convertible<InterfaceType*, IUnknown*>::value>>
	inline void Release(InterfaceType** obj)
	{
		if (*obj)
		{
			(*obj)->Release();
			*obj = nullptr;
		}
	}

	/// <summary>
	/// Acquires an additional reference, if non-null
	/// </summary>
	/// <typeparam name="InterfaceType">Is any type which inherits from IUnknown</typeparam>
	/// <param name="obj">Is the interface to add a reference to</param>
	/// <returns>The interface object passed into the function</returns>
	template<typename InterfaceType, typename = std::enable_if_t<std::is_convertible<InterfaceType*, IUnknown*>::value>>
	inline InterfaceType* Aquire(InterfaceType* obj)
	{
		if (obj)
			obj->AddRef();

		return obj;
	}

	/// <summary>
	/// Sets a new COM object, acquiring the reference
	/// </summary>
	/// <typeparam name="InterfaceType">Is any type which inherits from IUnknown</typeparam>
	/// <param name="currentObj">Is the object which will be the newObj</param>
	/// <param name="newObj">Is the new interface</param>
	template<typename InterfaceType, typename = std::enable_if_t<std::is_convertible<InterfaceType*, IUnknown*>::value>>
	inline void Attach(InterfaceType** currentObj, InterfaceType* newObj)
	{
		Util::Release(currentObj);
		*currentObj = newObj;
	}

	/// <summary>
	/// Releases a COM object and nullifies pointer
	/// </summary>
	/// <typeparam name="InterfaceType">Is any type which inherits from IUnknown</typeparam>
	/// <param name="obj">Is the interface</param>
	/// <returns>The old interface</returns>
	template<typename InterfaceType, typename = std::enable_if_t<std::is_convertible<InterfaceType*, IUnknown*>::value>>
	inline InterfaceType* Detach(InterfaceType** obj)
	{
		InterfaceType* oldObj = *obj;
		*obj = nullptr;
		return oldObj;
	}
}


