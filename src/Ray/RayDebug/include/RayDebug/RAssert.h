#pragma once

#ifndef NDEBUG

#include "RLogger.h"
#include <../../RayUtil/include/RayUtil/TypeConvert.h>
#include <../../RayUtil/include/RayUtil/Serialize.h>

#include <assert.h>
#include "RDBase.h"


namespace At0::Ray::Debug
{
	class RD_API RlAssert
	{
	public:
		template<typename... Args>
		static std::wstring AssertW(const std::string& str, Args&&... args)
		{
			std::wstring assertStr = Util::MultiByteToWideChar(Util::SerializeString(str, std::forward<Args>(args)...));
			RAY_LOG_CRITICAL("Assertion Failed with Message: {0}", assertStr);
			return assertStr;
		}

		template<typename... Args>
		static std::string AssertA(const std::string& str, Args&&... args)
		{
			std::string assertStr = Util::SerializeString(str, std::forward<Args>(args)...);
			RAY_LOG_CRITICAL("Assertion Failed with Message: {0}", assertStr);
			return assertStr;
		}
	};
}

#define RAY_WIDEC2(x) L ## x
#define RAY_WIDE(x) RAY_WIDEC2(x)

#ifdef _WIN32
#	define RAY_ASSERT(condition, msg, ...)	if(!(condition)) _wassert(::At0::Ray::Debug::RlAssert::AssertW(msg, __VA_ARGS__).c_str(), RAY_WIDE(__FILE__), (unsigned int)__LINE__)
#elif defined(__linux__)
	#define RAY_ASSERT(condition, msg, ...)	if(!(condition)) __assert(::At0::Ray::Debug::RlAssert::AssertA(msg, ## __VA_ARGS__).c_str(), __FILE__, (unsigned int)__LINE__)
#endif
	#define RAY_MEXPECTS(expected, msg)		RAY_ASSERT(expected, "[Expected] {0}\n{1}", #expected, msg)
	#define RAY_EXPECTS(expected)			RAY_ASSERT(expected, "[Expected] {0}", #expected)
#else
	#define RAY_ASSERT(condition, msg, ...)
	#define RAY_MEXPECTS(expected, msg)
	#define RAY_EXPECTS(expected)
#endif
