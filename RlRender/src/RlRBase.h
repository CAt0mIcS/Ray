#pragma once

#include <sal.h>

#ifdef RLR_BUILD
	#define RLR_API __declspec(dllexport)
#else
	#define RLR_API __declspec(dllimport)
#endif
