#ifndef RLU_TYPECONVERT_H
#define RLU_TYPECONVERT_H

#include <string>

#include "RlUBase.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <glm/mat4x4.hpp>


namespace At0::Util
{
	/// <summary>
	/// Converts a multi byte string (std::string) to wide character string (std::wstring)
	/// </summary>
	/// <param name="str">Is the multi byte string</param>
	/// <returns>The wide character string</returns>
	RLU_API std::wstring MultiByteToWideChar(const std::string_view str);

	/// <summary>
	/// Converts a wide character string (std::wstring) to multi byte string (std::string)
	/// </summary>
	/// <param name="str">Is the wide character string</param>
	/// <returns>The multi byte string</returns>
	RLU_API std::string WideCharToMultiByte(const std::wstring_view str);


#ifdef _WIN32
	/// <summary>
	/// Converts a transformation matrix to a windows rect structure
	/// </summary>
	/// <param name="matrix">Is the matrix to convert</param>
	/// <returns>The actual screen position of the transformed object</returns>
	RLU_API RECT TransformationToRect(const glm::mat4& matrix);

#endif
}

#endif // RLU_TYPECONVERT_H
