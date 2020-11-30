#pragma once

#ifndef NDEBUG

#include "ReyalLogger.h"
#include <../../RlUtilities/include/RlUtil/TypeConvert.h>
#include <../../RlUtilities/include/RlUtil/Serialize.h>

#include <assert.h>
#include "RlDBase.h"


namespace At0::Reyal::Debug
{
	class RLD_API RlAssert
	{
	public:
		template<typename... Args>
		static std::wstring AssertW(const std::string& str, Args&&... args)
		{
			std::wstring assertStr = Util::MultiByteToWideChar(Util::SerializeString(str, std::forward<Args>(args)...));
			RL_LOG_CRITICAL("Assertion Failed with Message: {0}", assertStr);
			return assertStr;
		}

		template<typename... Args>
		static std::string AssertA(const std::string& str, Args&&... args)
		{
			std::string assertStr = Util::SerializeString(str, std::forward<Args>(args)...);
			RL_LOG_CRITICAL("Assertion Failed with Message: {0}", assertStr);
			return assertStr;
		}
	};
}

#define RL_WIDEC2(x) L ## x
#define RL_WIDE(x) RL_WIDEC2(x)

#ifdef _WIN32
#	define RL_ASSERT(condition, msg, ...)	if(!(condition)) _wassert(::At0::Reyal::Debug::RlAssert::AssertW(msg, __VA_ARGS__).c_str(), RL_WIDE(__FILE__), (unsigned int)__LINE__)
#elif defined(__linux__)
	#define RL_ASSERT(condition, msg, ...)	if(!(condition)) __assert(::At0::Reyal::Debug::RlAssert::AssertA(msg, ## __VA_ARGS__).c_str(), __FILE__, (unsigned int)__LINE__)
#endif
	#define RL_MEXPECTS(expected, msg)		RL_ASSERT(expected, "[Expected] {0}\n{1}", #expected, msg)
	#define RL_EXPECTS(expected)			RL_ASSERT(expected, "[Expected] {0}", #expected)
#else
	#define RL_ASSERT(condition, msg, ...)
	#define RL_MEXPECTS(expected, msg)
	#define RL_EXPECTS(expected)
#endif

