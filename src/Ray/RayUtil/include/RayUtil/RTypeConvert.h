#pragma once

#include <string>

#include "RUBase.h"

#ifdef _WIN32
#include <Windows.h>
#include <../../extern/glm/glm/mat4x4.hpp>
#endif


namespace At0::Util
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


#ifdef _WIN32
	/// <summary>
	/// Converts a transformation matrix to a windows rect structure
	/// </summary>
	/// <param name="matrix">Is the matrix to convert</param>
	/// <returns>The actual screen position of the transformed object</returns>
	RU_API RECT TransformationToRect(const glm::mat4& matrix);

#endif
}

