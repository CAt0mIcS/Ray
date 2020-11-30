#pragma once

#include <string>

#include "RlLogger/LogLevel.h"


namespace At0::Log
{
	class Formatter
	{
	public:
		/// <summary>
		/// Virtual function which formats the string acording to implementation
		/// </summary>
		/// <param name="str">Is the string to format</param>
		/// <param name="logLvl">Is the current log level</param>
		virtual void Format(std::string& str, LogLevel logLvl) = 0;

		/// <summary>
		/// Default virtual Formatter Deconstructor
		/// </summary>
		virtual ~Formatter() = default;
	protected:
		/// <summary>
		/// Protected default Formatter Constructor
		/// </summary>
		Formatter() = default;
	};
}

