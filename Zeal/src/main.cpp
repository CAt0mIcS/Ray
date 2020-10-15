#include "ZealLogger/ZealLog.h"
int one = 32;
int two = 323;

void LogStuff(Zeal::Log::FileLogger& logger)
{
	for (int i = 0; i < 5000; ++i)
	{
		logger.Trace(L"This is a test {0}, {1}", one, two);
	}
}


int main()
{
	Zeal::Log::FileLogger logger(L"Zeal.log");
	logger.SetLogLevel(Zeal::Log::LogLevel::Trace);
	logger.Open();

	std::thread t1([&]()
		{
			LogStuff(logger);
		}
	);

	std::thread t2([&]()
		{
			LogStuff(logger);
		}
	);

	std::thread t3([&]()
		{
			LogStuff(logger);
		}
	);

	std::thread t4([&]()
		{
			LogStuff(logger);
		}
	);

	LogStuff(logger);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	logger.Close();
}