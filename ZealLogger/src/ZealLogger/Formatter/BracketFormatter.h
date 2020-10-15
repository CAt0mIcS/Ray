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

#ifndef ZEAL_NO_BRACKETS

			str.insert(0, "[] ");

#endif // !ZEAL_NO_BRACKETS
		}
	};
}
