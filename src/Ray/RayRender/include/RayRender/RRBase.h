#pragma once

#include <../../RayUtil/include/RayUtil/RGlobalDefines.h>

#ifndef NOMINMAX
#define NOMINMAX
#endif


#ifdef RR_BUILD
	#if defined(_MSC_VER)
		#define RR_API __declspec(dllexport)
	#else
		#define RR_API
	#endif
#else
	#if defined(_MSC_VER)
		#define RR_API __declspec(dllimport)
	#else
		#define RR_API
	#endif
#endif



