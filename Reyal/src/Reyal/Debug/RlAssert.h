#pragma once

#ifdef RL_DEBUG

#include "RlBase.h"
#include "ReyalLogger.h"


namespace At0::Reyal
{
	/// <summary>
	/// QUESTION:
	///		I want to move Assertion to ZlUtil, but then I'd need the logger in ZlUtil  ---> Needs to link to ZealLogger in ZlUtil?
	/// </summary>

	class RlAssert
	{
	public:
		template<typename... Args>
		static std::wstring Assert(const std::string& str, Args&&... args)
		{
			std::wstring assertStr = Util::MultiByteToWideChar(Util::SerializeString(str, std::forward<Args>(args)...));
			ZL_LOG_CRITICAL("Assertion Failed with Message: {0}", assertStr);
			return assertStr;
		}
	};
}


#define RL_ASSERT(condition, msg, ...)	if(!(condition)) _wassert(::At0::Reyal::RlAssert::Assert(msg, __VA_ARGS__).c_str(), _CRT_WIDE(__FILE__), (unsigned int)__LINE__)
#define RL_MEXPECTS(expected, msg)		RL_ASSERT(expected, "[Expected] {0}\n{1}", #expected, msg)
#define RL_EXPECTS(expected)			RL_ASSERT(expected, "[Expected] {0}", #expected)
#else
#define RL_ASSERT(condition, msg, ...)
#define RL_MEXPECTS(expected, msg)
#define RL_EXPECTS(expected)
#endif