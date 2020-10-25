#pragma once

#include <string>
#include "RlBase.h"


namespace Zeal::Reyal
{
	/// <summary>
	/// Converts a multi byte string (std::string) to wide character string (std::wstring)
	/// </summary>
	/// <param name="str">Is the multi byte string</param>
	/// <returns>The wide character string</returns>
	RL_API std::wstring MultiByteToWideChar(const std::string_view str);

	/// <summary>
	/// Converts a wide character string (std::wstring) to multi byte string (std::string)
	/// </summary>
	/// <param name="str">Is the wide character string</param>
	/// <returns>The multi byte string</returns>
	RL_API std::string WideCharToMultiByte(const std::wstring_view str);
}

