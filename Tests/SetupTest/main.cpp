#include <Utils/RLogger.h>


using namespace At0;

int main()
{
	Ray::Log::SetLogLevel(Ray::LogLevel::Trace);
	Ray::Log::Open("../../Ray.log");
	Ray::Log::Debug("Hello world\n");
}
