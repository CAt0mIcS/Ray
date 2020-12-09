#pragma once

#include <../../RayUtil/include/RayUtil/Helper.h>

#ifndef NOMINMAX
	#define NOMINMAX
#endif


#ifdef RLR_BUILD
	#if defined(_MSC_VER)
		#define RLR_API __declspec(dllexport)
	#elif defined(__GNUC__) || defined(__clang__)
		#define RLR_API __attribute__((dllexport))
	#else
		#define RLR_API
	#endif
#else
	#if defined(_MSC_VER)
		#define RLR_API __declspec(dllimport)
	#elif defined(__GNUC__) || defined(__clang__)
		#define RLR_API __attribute__((dllimport))
	#else
		#define RLR_API
	#endif
#endif


namespace At0::Ray
{
	enum class RLR_API FileState
	{
		Compiled = 0, Source
	};
}

