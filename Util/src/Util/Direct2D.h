#pragma once

namespace DWrite
{
	/**
	* QUESTION:
	*	Should I do this?
	*	Should I use "typedef" or "using"?
	*	Should I line the "=" up?
	*/

	using FontWeight		= DWRITE_FONT_WEIGHT;
	using FontStyle			= DWRITE_FONT_STYLE;
	using FontStretch		= DWRITE_FONT_STRETCH;
	using MeasuringMode		= DWRITE_MEASURING_MODE;

	using TextMetrics		= DWRITE_TEXT_METRICS;
	using LineMetrics		= DWRITE_LINE_METRICS;
	using ClusterMetrics	= DWRITE_CLUSTER_METRICS;
	using OverhangMetrics	= DWRITE_OVERHANG_METRICS;
	using HitTestMetrics	= DWRITE_HIT_TEST_METRICS;

	using TextRange			= DWRITE_TEXT_RANGE;

	using Matrix			= DWRITE_MATRIX;
}

namespace Direct2D
{
	using RectF				= D2D1_RECT_F;
	using DrawTextOptions	= D2D1_DRAW_TEXT_OPTIONS;

	using Point2F			= D2D1_POINT_2F;
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


