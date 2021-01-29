#include <Core/RApplication.h>
#include <Debug/RLogger.h>
#include <Devices/RWindow.h>

using namespace At0;


class App : public Ray::Application
{
public:
private:
};


int main()
{
	Ray::Log::Open("../../Ray.log");
	Ray::Log::SetLogLevel(Ray::LogLevel::Trace);

	App app;

	Ray::Window::Create();
	Ray::Window::Get().Show();

	return app.Run();
}
