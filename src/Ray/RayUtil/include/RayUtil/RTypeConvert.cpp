#include "Rupch.h"
#include "RTypeConvert.h"

//RAY_TODO: Debug
#include <iostream>


namespace At0::Ray::Util
{
#ifdef _WIN32

	std::wstring MultiByteToWideChar(std::string_view str)
	{
		auto size = ::MultiByteToWideChar(CP_UTF8, MB_COMPOSITE, str.data(), -1, nullptr, 0);

		std::wstring buff;
		buff.resize(size);
		::MultiByteToWideChar(CP_UTF8, MB_COMPOSITE, str.data(), -1, buff.data(), size);

		return buff;
	}

	std::string WideCharToMultiByte(const std::wstring_view str)
	{
		auto size = ::WideCharToMultiByte(CP_UTF8, WC_COMPOSITECHECK, str.data(), -1, nullptr, 0, NULL, NULL);

		std::string buff;
		buff.resize(size);
		::WideCharToMultiByte(CP_UTF8, WC_COMPOSITECHECK, str.data(), -1, buff.data(), size, NULL, NULL);

		return buff;
	}

#elif defined(__linux__)

	std::wstring MultiByteToWideChar(std::string_view str)
	{
		std::wstring buff;
		buff.resize(str.size());

		if (mbstowcs(buff.data(), str.data(), str.size()) == (size_t)-1)
		{
			perror("mbstowcs");
			return L"";
		}

		std::wcout << L"ConversionResult: " << buff << L'\n';
		return buff;
	}

	std::string WideCharToMultiByte(const std::wstring_view str)
	{
		std::string buff;
		buff.resize(str.size());

		if (wcstombs(buff.data(), str.data(), str.size()) == (size_t)-1)
		{
			perror("wcstombs");
			return "";
		}

		std::cout << "ConversionResult: " << buff << '\n';
		return buff;
	}

#endif

}



