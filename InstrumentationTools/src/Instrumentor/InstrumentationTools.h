#pragma once

#define ZEAL_ENABLE_PROFILING 1

#if ZEAL_ENABLE_PROFILING

#include "Instrumentor.h"

#endif


#if ZEAL_ENABLE_PROFILING

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define ZEAL_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define ZEAL_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define ZEAL_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define ZEAL_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define ZEAL_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define ZEAL_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define ZEAL_FUNC_SIG __func__
#else
#define ZEAL_FUNC_SIG "HZ_FUNC_SIG unknown!"
	#endif

#endif


#if ZEAL_ENABLE_PROFILING
	#define ZEAL_PROFILE_BEGIN_SESSION(name, filepath)	::Zeal::Instrumentation::Instrumentor::Get().BeginSession(name, filepath)
	#define ZEAL_PROFILE_END_SESSION()					::Zeal::Instrumentation::Instrumentor::Get().EndSession()
	#define ZEAL_PROFILE_LINE_2(name, line)				::Zeal::Instrumentation::Timer timer__ZEAL_##line(name)
	#define ZEAL_PROFILE_LINE(name, line)				ZEAL_PROFILE_LINE_2(name, line)
	#define ZEAL_PROFILE_SCOPE(name)					ZEAL_PROFILE_LINE(name, __LINE__)
	#define ZEAL_PROFILE_FUNCTION()						ZEAL_PROFILE_SCOPE(ZEAL_FUNC_SIG)
#else
	#define ZEAL_PROFILE_BEGIN_SESSION(name, filepath)
	#define ZEAL_PROFILE_END_SESSION()
	#define ZEAL_PROFILE_SCOPE(name)
	#define ZEAL_PROFILE_FUNCTION()
#endif
