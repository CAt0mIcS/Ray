#pragma once

#include <string>

#include "RUBase.h"

#ifdef _WIN32
#include <Windows.h>
#endif


namespace At0::Ray::Util
{
	/// <summary>
	/// Converts a multi byte string (std::string) to wide character string (std::string)
	/// </summary>
	/// <param name="str">Is the multi byte string</param>
	/// <returns>The wide character string</returns>
	RU_API std::wstring MultiByteToWideChar(std::string_view str);

	/// <summary>
	/// Converts a wide character string (std::string) to multi byte string (std::string)
	/// </summary>
	/// <param name="str">Is the wide character string</param>
	/// <returns>The multi byte string</returns>
	RU_API std::string WideCharToMultiByte(const std::wstring_view str);
}

