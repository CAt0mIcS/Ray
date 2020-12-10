#pragma once

#include <../../RayUtil/include/RayUtil/Helper.h>

#ifndef NOMINMAX
	#define NOMINMAX
#endif


#ifdef RR_BUILD
	#if defined(_MSC_VER)
		#define RR_API __declspec(dllexport)
	#elif defined(__GNUC__) || defined(__clang__)
		#define RR_API __attribute__((dllexport))
	#else
		#define RR_API
	#endif
#else
	#if defined(_MSC_VER)
		#define RR_API __declspec(dllimport)
	#elif defined(__GNUC__) || defined(__clang__)
		#define RR_API __attribute__((dllimport))
	#else
		#define RR_API
	#endif
#endif


namespace At0::Ray
{
	enum class RR_API FileState
	{
		Compiled = 0, Source
	};
}

