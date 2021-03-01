#include <Core/REngine.h>
#include <Utils/RLogger.h>
#include <Devices/RWindow.h>
#include <Graphics/RGraphics.h>

#include <Components/RMesh.h>
#include <Components/RTransform.h>
#include <Components/RModel.h>

#include <Graphics/RVertex.h>
#include <Scene/RScene.h>

#include <signal.h>
#include <random>


using namespace At0;


class Scene : public Ray::Scene
{
public:
	Scene() {}
};


class App : public Ray::Engine, Ray::EventListener<Ray::MouseButtonPressedEvent>
{
public:
	App() { Scene::Set(Ray::MakeScope<Scene>()); }

private:
	void Update() override {}

	void OnEvent(Ray::MouseButtonPressedEvent& e) override
	{
		static std::mt19937 device;
		static std::uniform_real_distribution<float> posRotDist(-30.0f, 30.0f);
		static std::uniform_real_distribution<float> scaleDist(0.2f, 2.5f);
		static uint32_t modelCount = 0;

		for (uint32_t i = 0; i < 1; ++i)
		{
			auto modelView = Scene::Get().EntityView<Ray::Model>();

			Ray::Entity& entity = Ray::Scene::Get().CreateEntity();
			// Ray::Mesh& mesh =
			// entity.Emplace<Ray::Mesh>(Ray::Mesh::Cube(Ray::Material::Default())); auto&
			// cubeTransform = mesh.Get<Ray::Transform>(); cubeTransform.Translation = {
			// posRotDist(device), posRotDist(device), 	posRotDist(device) }; cubeTransform.Rotation
			// = { posRotDist(device), posRotDist(device), posRotDist(device) }; cubeTransform.Scale
			// = { scaleDist(device), scaleDist(device), scaleDist(device) };

			Ray::Model& model =
				entity.Emplace<Ray::Model>("Resources/Models/Nanosuit/nanosuit.obj");
			auto& modelTransform = model.Get<Ray::Transform>();
			// modelTransform.Translation = { posRotDist(device), posRotDist(device),
			//	posRotDist(device) };
			// modelTransform.Rotation = { posRotDist(device), posRotDist(device),
			// posRotDist(device)
			// }; modelTransform.Scale = { scaleDist(device), scaleDist(device), scaleDist(device)
			// };

			++modelCount;
		}
	}
};

void SignalHandler(int signal)
{
	Ray::Log::Critical("Signal {0} received", signal);
	Ray::Log::Close();
}

int main()
{
	signal(SIGABRT, SignalHandler);
	signal(SIGILL, SignalHandler);
	signal(SIGINT, SignalHandler);

	Ray::Log::Open("Ray.log");
	Ray::Log::SetLogLevel(Ray::LogLevel::Trace);

	Ray::Window::Create();
	Ray::Window::Get().Show();

	App app;

	Ray::Graphics::Get();

	return app.Run();
}
