#include <Core/REngine.h>
#include <Utils/RLogger.h>
#include <Devices/RWindow.h>
#include <Graphics/RGraphics.h>

using namespace At0;


class App : public Ray::Engine
{
public:
private:
	void Update() override
	{
		// std::cout << Engine::Get().GetDelta().AsSeconds() << '\n';
		// std::cout << Engine::Get().GetFPS() << '\n';
	}
};


int main()
{
	Ray::Log::Open("../../Ray.log");
	Ray::Log::SetLogLevel(Ray::LogLevel::Trace);

	App app;

	Ray::Window::Create();
	Ray::Window::Get().Show();
	Ray::Graphics::Get();

	return app.Run();
}
