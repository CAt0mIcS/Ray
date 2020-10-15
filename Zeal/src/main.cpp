#include "ZealLogger/ZealLog.h"
#include "Instrumentor/InstrumentationTools.h"

int one = 32;
int two = 323;

void LogStuff()
{
	ZEAL_PROFILE_FUNCTION();
	for (int i = 0; i < 5000; ++i)
	{
		//logger.Trace(L"This is a test {0}, {1}", one, two);
	}
}


int main()
{
	Zeal::Log::Logger::Init("Zeal.log");
	Zeal::Log::Logger::Get()->SetLogLevel(Zeal::Log::LogLevel::Trace);

	//std::thread t1([&]()
	//	{
	//		LogStuff();
	//	}
	//);

	//std::thread t2([&]()
	//	{
	//		LogStuff();
	//	}
	//);

	//std::thread t3([&]()
	//	{
	//		LogStuff();
	//	}
	//);

	//std::thread t4([&]()
	//	{
	//		LogStuff();
	//	}
	//);

	LogStuff();

	//t1.join();
	//t2.join();
	//t3.join();
	//t4.join();
}