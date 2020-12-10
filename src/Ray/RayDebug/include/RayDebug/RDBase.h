#pragma once

#ifdef RD_BUILD
	#if defined(_MSC_VER)
		#define RD_API __declspec(dllexport)
	#elif defined(__GNUC__) || defined(__clang__)
		#define RD_API __attribute__((dllexport))
	#else
		#define RD_API
	#endif
#else
	#if defined(_MSC_VER)
		#define RD_API __declspec(dllimport)
	#elif defined(__GNUC__) || defined(__clang__)
		#define RD_API __attribute__((dllimport))
	#else
		#define RD_API
	#endif
#endif

