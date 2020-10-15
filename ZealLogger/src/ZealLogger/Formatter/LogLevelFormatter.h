#pragma once

#include "Formatter.h"


namespace Zeal::Log
{
	class LogLevelFormatter : public Formatter
	{
	public:
		LogLevelFormatter() = default;

		virtual void Format(std::wstring& str, LogLevel logLvl) override
		{
#ifndef ZEAL_NO_LOG_LEVEL_OUT

			str.insert(1, s_LogLevelStr[(int)logLvl]);

#endif // !ZEAL_NO_LOG_LEVEL_OUT
		}

	private:
#ifndef ZEAL_NO_LOG_LEVEL_OUT

		inline static const std::wstring s_LogLevelStr[] = 
		{
			L"TRC", L"DBG", L"LOG", L"WRN", L"ERR", L"CRT", L"UDF"
		};

#endif // !ZEAL_NO_LOG_LEVEL_OUT


	};
}
