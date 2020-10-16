#pragma once

#include <string>

#include "../Private/LogLevel.h"


namespace Zeal::Log
{
	class Formatter
	{
	public:
		virtual void Format(std::string& str, LogLevel logLvl) = 0;

	protected:
		Formatter() = default;
	};
}
