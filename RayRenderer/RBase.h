#pragma once

// clang-format off

#ifdef __cplusplus
#define RR_EXTERN_C_BEG extern "C" {
#define RR_EXTERN_C_END }
#else
#define RR_EXTERN_C_BEG
#define RR_EXTERN_C_END
#endif

RR_EXTERN_C_BEG

#define RR_RENDERER_API_VULKAN 0
#define RR_RENDERER_API_OPENGL 1

#ifndef RR_RENDERER_API
	#define RR_RENDERER_API RR_RENDERER_API_OPENGL
#endif

#ifdef _MSC_VER
	#ifdef RR_BUILD
		#define RR_API __declspec(dllexport)
	#else
		#define RR_API _declspec(dllimport)
	#endif
#else
	#define RR_API
#endif


#include <stdint.h>
#include <stdbool.h>


RR_EXTERN_C_END
