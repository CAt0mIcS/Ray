#include <Core/RApplication.h>
#include <Devices/RWindow.h>

#include <Utils/RLogger.h>

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
