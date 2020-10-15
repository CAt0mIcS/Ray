#pragma once

#include <locale>
#include <codecvt>
#include <string>


namespace Zeal::Log::Helper
{
	std::string ToMultiByte(const std::wstring& str)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.to_bytes(str);
	}
}


