#pragma once

#define UTIL_DYNAMIC_LINK_LIBRARY 0

#if UTIL_DYNAMIC_LINK_LIBRARY
	#ifdef UTIL_EXPORT
		#define UTIL_API __declspec(dllexport)
	#else
		#define UTIL_API __declspec(dllimport)
	#endif
#else
	#define UTIL_API
#endif