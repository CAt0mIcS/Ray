#include <Core/REngine.h>
#include <Utils/RLogger.h>
#include <Devices/RWindow.h>
#include <Graphics/RGraphics.h>

#include <Core/RVertex.h>
#include <Core/RScene.h>

#include <signal.h>


using namespace At0;


class App : public Ray::Engine
{
public:
	App() {}

private:
	void Update() override {}
};

void __CRTDECL SignalHandler(int signal)
{
	Ray::Log::Critical("Signal {0} received", signal);
	Ray::Log::Close();
}

int main()
{
	signal(SIGABRT, SignalHandler);
	signal(SIGILL, SignalHandler);
	signal(SIGINT, SignalHandler);
	signal(SIGBREAK, SignalHandler);

	Ray::Log::Open("Ray.log");
	Ray::Log::SetLogLevel(Ray::LogLevel::Trace);

	App app;

	Ray::Window::Create();
	Ray::Window::Get().Show();
	Ray::Graphics::Get();

	return app.Run();
}
