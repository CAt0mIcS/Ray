#ifndef RLR_RLRBASE_H
#define RLR_RLRBASE_H

#ifndef NOMINMAX
	#define NOMINMAX
#endif

#ifdef RLR_BUILD
	#if defined(_MSC_VER)
		#define RLR_API __declspec(dllexport)
	#elif defined(__GNUC__) || defined(__clang__)
		#define RLR_API __attribute__((dllexport))
	#else
		#define RLR_API
	#endif
#else
	#if defined(_MSC_VER)
		#define RLR_API __declspec(dllimport)
	#elif defined(__GNUC__) || defined(__clang__)
		#define RLR_API __attribute__((dllimport))
	#else
		#define RLR_API
	#endif
#endif

#endif // RLR_RLRBASE_H