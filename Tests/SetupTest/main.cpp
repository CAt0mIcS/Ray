#include <Core/REngine.h>
#include <Utils/RLogger.h>
#include <Devices/RWindow.h>
#include <Graphics/RGraphics.h>

#include <Components/RMesh.h>
#include <Components/RTransform.h>

#include <Core/RVertex.h>
#include <Core/RScene.h>

#include <signal.h>


using namespace At0;


class App : public Ray::Engine, Ray::EventListener<Ray::MouseButtonPressedEvent>
{
public:
	App() {}

private:
	void Update() override {}

	void OnEvent(Ray::MouseButtonPressedEvent& e) override
	{
		Ray::Entity& entity = Ray::Scene::Get().CreateEntity();

		// RAY_TODO: Adding the transform first causes vector out of range subscription when
		// updating the mesh in Scene::Update
		entity.Emplace<Ray::Mesh>(Ray::Mesh::Triangle(Ray::Material::Default()));
		entity.Emplace<Ray::Transform>();
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
