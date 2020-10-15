#pragma once

#include "Formatter.h"


namespace Zeal::Log
{
	class LogLevelFormatter : public Formatter
	{
	public:
		LogLevelFormatter() = default;

		virtual void Format(std::string& str, LogLevel logLvl) override
		{
#ifndef ZEAL_NO_LOG_LEVEL_OUT

			str.insert(1, s_LogLevelStr[(int)logLvl]);

#endif // !ZEAL_NO_LOG_LEVEL_OUT
		}

	private:
#ifndef ZEAL_NO_LOG_LEVEL_OUT

		inline static const std::string s_LogLevelStr[] = 
		{
			"TRC", "DBG", "LOG", "WRN", "ERR", "CRT", "UDF"
		};

#endif // !ZEAL_NO_LOG_LEVEL_OUT


	};
}
