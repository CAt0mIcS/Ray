#include <Core/RApplication.h>
#include <Devices/RWindow.h>

#include <Utils/RLogger.h>


using namespace At0;

int main()
{
	Ray::Log::Open("../../Ray.log");
	Ray::Log::SetLogLevel(Ray::LogLevel::Trace);

	Ray::Window::Create();
	Ray::Window::Get().Show();

	return Ray::Application::Get().Run();
}
