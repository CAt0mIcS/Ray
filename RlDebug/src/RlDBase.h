#pragma once

#ifdef RLD_BUILD
	#define RLD_API __declspec(dllexport)
#else
	#define RLD_API __declspec(dllimport)
#endif
