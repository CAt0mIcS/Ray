#include "Rupch.h"
#include "RHelper.h"

#include <assert.h>
#include <iostream>


namespace At0::Ray::Util
{
	void AllocateConsole()
	{
#if defined(_WIN32)
		AllocConsole();

		// std::cout, std::clog, std::cerr, std::cin;
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		freopen("CONIN$", "r", stdin);

		// std::wcout, std::wclog, std::wcerr, std::wcin;
		HANDLE hConOut = CreateFileA("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		HANDLE hConIn = CreateFileA("CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
		SetStdHandle(STD_ERROR_HANDLE, hConOut);
		SetStdHandle(STD_INPUT_HANDLE, hConIn);
#endif
	}

	void DeallocateConsole()
	{
#if defined(_WIN32)
		std::cout.clear();
		std::clog.clear();
		std::cerr.clear();
		std::cin.clear();

		std::wcout.clear();
		std::wclog.clear();
		std::wcerr.clear();
		std::wcin.clear();

		FreeConsole();
#endif
	}

	bool EndsWith(std::string_view str, std::string_view end)
	{
		assert(str.size() > end.size() && "[Util::EndsWith] The supposed end of the string is larger than it.");
		return str.substr(str.size() - end.size(), str.size()) == end;
	}
}

