#pragma once

#include <locale>
#include <codecvt>
#include <string>


namespace Zeal::Log::Helper
{
	std::wstring ToWideChar(const std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(str);
	}
}


