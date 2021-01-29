#include <Core/REngine.h>
#include <Debug/RLogger.h>
#include <Devices/RWindow.h>

using namespace At0;


class App : public Ray::Engine
{
public:
private:
	void Update() override
	{ /*std::cout << Engine::Get().GetDelta().AsSeconds() << '\n';*/
	}
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
