#ifndef ZZL_TYPECONVERT_H
#define ZZL_TYPECONVERT_H

#include <string>


namespace At0::Log::Util
{
	/// <summary>
	/// Converts a multi byte string (std::string) to wide character string (std::wstring)
	/// </summary>
	/// <param name="str">Is the multi byte string</param>
	/// <returns>The wide character string</returns>
	std::wstring MultiByteToWideChar(const std::string_view str);

	/// <summary>
	/// Converts a wide character string (std::wstring) to multi byte string (std::string)
	/// </summary>
	/// <param name="str">Is the wide character string</param>
	/// <returns>The multi byte string</returns>
	std::string WideCharToMultiByte(const std::wstring_view str);
}

#endif /* ZZL_TyPECONVERT_H */