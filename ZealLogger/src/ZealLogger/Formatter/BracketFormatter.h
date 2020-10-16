#pragma once

#include "Formatter.h"


namespace Zeal::Log
{
	class BracketFormatter : public Formatter
	{
	public:
		BracketFormatter() = default;

		virtual void Format(std::string& str, LogLevel logLvl) override
		{
			ZL_PROFILE_FUNCTION();

#ifndef ZL_NO_BRACKETS

			str.insert(0, "[] ");

#endif // !ZL_NO_BRACKETS
		}
	};
}
