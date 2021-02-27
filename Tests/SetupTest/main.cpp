#include <Core/REngine.h>
#include <Utils/RLogger.h>
#include <Devices/RWindow.h>
#include <Graphics/RGraphics.h>

#include <Components/RMesh.h>
#include <Components/RTransform.h>

#include <Core/RVertex.h>
#include <Core/RScene.h>

#include <signal.h>
#include <random>


using namespace At0;


class App : public Ray::Engine, Ray::EventListener<Ray::MouseButtonPressedEvent>
{
public:
	App() {}

private:
	void Update() override {}

	void OnEvent(Ray::MouseButtonPressedEvent& e) override
	{
		static std::mt19937 device;
		static std::uniform_real_distribution<float> posRotDist(-50.0f, 50.0f);
		static std::uniform_real_distribution<float> scaleDist(0.2f, 2.5f);

		for (uint32_t i = 0; i < 10000; ++i)
		{
			Ray::Entity& entity = Ray::Scene::Get().CreateEntity();
			entity.Emplace<Ray::Mesh>(Ray::Mesh::Triangle(Ray::Material::Default()));
			auto& transform = entity.Get<Ray::Transform>();
			transform.Translation = { posRotDist(device), posRotDist(device), posRotDist(device) };
			transform.Rotation = { posRotDist(device), posRotDist(device), posRotDist(device) };
			transform.Scale = { scaleDist(device), scaleDist(device), scaleDist(device) };
		}
	}
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

	Ray::Window::Create();
	Ray::Window::Get().Show();
	Ray::Graphics::Get();

	return App{}.Run();
}
