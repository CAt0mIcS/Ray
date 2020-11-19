#ifndef RL_RLBASE_H
#define RL_RLBASE_H

#include <memory>
#include <assert.h>

// Define so Windows.h doesnt define min, max macros
#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifdef RL_BUILD
	#if defined(_MSC_VER)
		#define RL_API __declspec(dllexport)
	#elif defined(__GNUC__) || defined(__clang__)
		#define RL_API __attribute__((dllexport))
	#else
		#define RL_API
	#endif
#else
	#if defined(_MSC_VER)
		#define RL_API __declspec(dllimport)
	#elif defined(__GNUC__) || defined(__clang__)
		#define RL_API __attribute__((dllimport))
	#else
		#define RL_API
	#endif
#endif


#include <../../RlUtilities/include/RlUtil/Helper.h>


#endif // RL_RLBASE_H
