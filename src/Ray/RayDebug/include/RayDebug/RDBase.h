#pragma once

#ifdef RD_BUILD
	#if defined(_MSC_VER)
		#define RD_API __declspec(dllexport)
	#else
		#define RD_API
	#endif
#else
	#if defined(_MSC_VER)
		#define RD_API __declspec(dllimport)
	#else
		#define RD_API
	#endif
#endif

