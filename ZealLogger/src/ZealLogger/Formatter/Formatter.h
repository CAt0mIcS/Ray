#pragma once

#include <string>

#include "../ZealCustomize.h"
#include "../Private/LogLevel.h"


namespace Zeal::Log
{
	class Formatter
	{
	public:
		virtual void Format(std::wstring& str, LogLevel logLvl) = 0;

	protected:
		Formatter() = default;
	};
}
