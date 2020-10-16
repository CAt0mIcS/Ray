#pragma once



#ifdef RL_BUILD
	#define RL_API __declspec(dllexport)
#else
	#define RL_API __declspec(dllimport)
#endif
