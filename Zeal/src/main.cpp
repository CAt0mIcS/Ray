#include "pch.h"
#include <iostream>

int one = 32;
int two = 323;

void LogStuff()
{
	ZL_PROFILE_FUNCTION();
	std::string arg1 = "arg1";
	std::wstring arg2 = L"arg2";

	const wchar_t* arg3 = L"arg3";
	const char* arg4 = "arg4";

	for (uint32_t i = 0; i < 5000; ++i)
		ZL_LOG_DEBUG("This is log message {0}, {1}, {2}, {3}, {4}, {5}, {6}", i, L"Tesxt", "Fdsf", arg1, arg2, arg3, arg4);
}


int main()
{
	ZL_PROFILE_BEGIN_SESSION("Startup", "Startup.json");
	ZL_LOG_INIT("Zeal.log", Zeal::Log::LogLevel::Trace);


	std::thread t1([&]()
		{
			LogStuff();
		}
	);

	std::thread t2([&]()
		{
			LogStuff();
		}
	);

	std::thread t3([&]()
		{
			LogStuff();
		}
	);

	std::thread t4([&]()
		{
			LogStuff();
		}
	);

	LogStuff();

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	ZL_PROFILE_END_SESSION();
}