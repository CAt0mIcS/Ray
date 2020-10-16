#pragma once

#define ZL_ENABLE_PROFILING 1


/// <summary>
/// Define profiling macros here
/// </summary>
#if ZL_ENABLE_PROFILING

	#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
		#define ZL_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(__DMC__) && (__DMC__ >= 0x810)
		#define ZL_FUNC_SIG __PRETTY_FUNCTION__
	#elif (defined(__FUNCSIG__) || (_MSC_VER))
		#define ZL_FUNC_SIG __FUNCSIG__
	#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
		#define ZL_FUNC_SIG __FUNCTION__
	#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
		#define ZL_FUNC_SIG __FUNC__
	#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
		#define ZL_FUNC_SIG __func__
	#elif defined(__cplusplus) && (__cplusplus >= 201103)
		#define ZL_FUNC_SIG __func__
	#else
		#define ZL_FUNC_SIG "ZL_FUNC_SIG unknown!"
	#endif

#endif


#if ZL_ENABLE_PROFILING
	#define ZL_PROFILE_BEGIN_SESSION(name, filepath)	::Zeal::Instrumentation::Instrumentor::Get().BeginSession(name, filepath)
	#define ZL_PROFILE_END_SESSION()					::Zeal::Instrumentation::Instrumentor::Get().EndSession()
	#define ZL_PROFILE_LINE_2(name, line)				::Zeal::Instrumentation::Timer timer__ZEAL_##line(name)
	#define ZL_PROFILE_LINE(name, line)					ZL_PROFILE_LINE_2(name, line)
	#define ZL_PROFILE_SCOPE(name)						ZL_PROFILE_LINE(name, __LINE__)
	#define ZL_PROFILE_FUNCTION()						ZL_PROFILE_SCOPE(ZL_FUNC_SIG)
#else
	#define ZL_PROFILE_BEGIN_SESSION(name, filepath)
	#define ZL_PROFILE_END_SESSION()
	#define ZL_PROFILE_SCOPE(name)
	#define ZL_PROFILE_FUNCTION()
#endif