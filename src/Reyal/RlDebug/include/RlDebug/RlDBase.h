#pragma once

#ifdef RLD_BUILD
	#if defined(_MSC_VER)
		#define RLD_API __declspec(dllexport)
	#elif defined(__GNUC__) || defined(__clang__)
		#define RLD_API __attribute__((dllexport))
	#else
		#define RLD_API
	#endif
#else
	#if defined(_MSC_VER)
		#define RLD_API __declspec(dllimport)
	#elif defined(__GNUC__) || defined(__clang__)
		#define RLD_API __attribute__((dllimport))
	#else
		#define RLD_API
	#endif
#endif

