#include <Core/REngine.h>
#include <Utils/RLogger.h>
#include <Devices/RWindow.h>
#include <Graphics/RGraphics.h>

#include <Geometry/RMesh.h>
#include <Geometry/RModel.h>
#include <Graphics/Images/RTexture2D.h>
#include <Utils/RException.h>

#include <Graphics/RVertex.h>
#include <Scene/RScene.h>
#include <Scene/RCamera.h>

#include <signal.h>
#include <random>


using namespace At0;


class Scene : public Ray::Scene
{
public:
	// Creating the camera here wouldn't work!
	Scene() {}

	void Start() override
	{
		SetCamera(Ray::MakeScope<Ray::Camera>());
		Ray::UInt2 size = Ray::Window::Get().GetFramebufferSize();
		GetCamera().SetPosition(glm::vec3(0.0f, 0.0f, -2.5f));
		GetCamera().SetRotation(glm::vec3(0.0f));
		GetCamera().SetRotationSpeed(0.07f);
		GetCamera().SetPerspective(60.0f, (float)size.x / (float)size.y, 0.1f, 512.0f);
		GetCamera().SetMovementSpeed(3.0f);
	}
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
		// static std::uniform_real_distribution<float> posRotDist(-100.0f, 100.0f);
		static std::uniform_real_distribution<float> scaleDist(0.2f, 2.5f);
		static uint32_t modelCount = 0;

		for (uint32_t i = 0; i < 1; ++i)
		{
			Ray::Entity& entity = Ray::Scene::Get().CreateEntity();
			// Ray::Material texturedMaterial({ 1.0f, 1.0f, 1.0f, 1.0f }, nullptr, 0.0f, 0.0f,
			//	Ray::MakeRef<Ray::Texture2D>("Resources/Textures/gridbase.png"), nullptr, true,
			//	nullptr);
			// Ray::Material defaultMaterial(
			//	{ "Resources/Shaders/DefaultShader.vert", "Resources/Shaders/DefaultShader.frag" });

			// Ray::Mesh& mesh = entity.Emplace<Ray::Mesh>(Ray::Mesh::Triangle(defaultMaterial));

			// auto& cubeTransform = mesh.GetTransform();
			// cubeTransform.Translation = { posRotDist(device), posRotDist(device),
			//	posRotDist(device) };
			// cubeTransform.Rotation
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
	Ray::Log::SetLogLevel(Violent::LogLevel::Information);

	try
	{
		Ray::Window::Create();
		Ray::Window::Get().Show();

		App app;

		Ray::Graphics::Get();

		return app.Run();
	}
	catch (Ray::Exception& e)
	{
		Ray::Log::Critical("{0}: {1}", e.GetType(), e.what());
	}
	catch (std::exception& e)
	{
		Ray::Log::Critical("Standard Exception: {0}", e.what());
	}
	catch (...)
	{
		Ray::Log::Critical("Unknown exception occured.");
	}
}
