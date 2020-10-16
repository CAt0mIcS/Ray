#include "utilpch.h"
#include "TypeConvert.h"


namespace Zeal::Util
{
	std::wstring MultiByteToWideChar(const std::string& str)
	{
		auto size = ::MultiByteToWideChar(CP_UTF8, MB_COMPOSITE, str.c_str(), -1, nullptr, 0);

		std::wstring buff;
		buff.resize(size);
		::MultiByteToWideChar(CP_UTF8, MB_COMPOSITE, str.c_str(), -1, buff.data(), size);

		return buff;
	}

	std::string WideCharToMultiByte(const std::wstring& str)
	{
		auto size = ::WideCharToMultiByte(CP_UTF8, WC_COMPOSITECHECK, str.c_str(), -1, nullptr, 0, NULL, NULL);

		std::string buff;
		buff.resize(size);
		::WideCharToMultiByte(CP_UTF8, WC_COMPOSITECHECK, str.c_str(), -1, buff.data(), size, NULL, NULL);

		return buff;
	}
}



