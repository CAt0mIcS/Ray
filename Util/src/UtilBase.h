#pragma once

#ifdef UTIL_EXPORT
	#define UTIL_API __declspec(dllexport)
#else
	#define UTIL_API __declspec(dllimport)
#endif
