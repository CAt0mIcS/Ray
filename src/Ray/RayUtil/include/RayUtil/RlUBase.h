#pragma once

// Defined so Windows.h doesn't define min, max macro

// QUESTIONA: Where to put this so it's global? (globaldefines.h / use project settings)
#ifndef NOMINMAX
#define NOMINMAX
#endif

#define RU_BUILD_AS_DLL 1

#if RU_BUILD_AS_DLL
	#ifdef RU_BUILD
		#if defined(_MSC_VER)
			#define RU_API __declspec(dllexport)
		#elif defined(__GNUC__) || defined(__clang__)
			#define RU_API __attribute__((dllexport))
		#else
			#define RU_API
		#endif
	#else
		#if defined(_MSC_VER)
			#define RU_API __declspec(dllimport)
		#elif defined(__GNUC__) || defined(__clang__)
			#define RU_API __attribute__((dllimport))
		#else
			#define RU_API
		#endif
	#endif
#else
	#define RU_API
#endif

