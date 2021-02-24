#include <Core/REngine.h>
#include <Utils/RLogger.h>
#include <Devices/RWindow.h>
#include <Graphics/RGraphics.h>

#include <Core/RVertex.h>
#include <Core/RScene.h>

using namespace At0;


class App : public Ray::Engine
{
public:
	App() {}

private:
	void Update() override {}
};


int main()
{
	Ray::Log::Open("Ray.log");
	Ray::Log::SetLogLevel(Ray::LogLevel::Trace);

	App app;

	Ray::Window::Create();
	Ray::Window::Get().Show();
	Ray::Graphics::Get();

	return app.Run();
}
