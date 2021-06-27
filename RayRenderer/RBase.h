#pragma once


#ifdef __cplusplus
	#define RR_EXTERN_C_BEG \
		extern "C"          \
		{
	#define RR_EXTERN_C_END }
#else
	#define RR_EXTERN_C_BEG
	#define RR_EXTERN_C_END
#endif


#ifdef _MSC_VER
	#ifdef RR_BUILD
		#define RR_API __declspec(dllexport)
	#else
		#define RR_API __declspec(dllimport)
	#endif
#else
	#define RR_API
#endif
