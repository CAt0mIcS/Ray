#include <Core/REngine.h>
#include <Utils/RLogger.h>
#include <Devices/RWindow.h>
#include <Graphics/RGraphics.h>

#include <Geometry/RMesh.h>
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
	Scene()
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


class App :
	public Ray::Engine,
	Ray::EventListener<Ray::MouseButtonPressedEvent>,
	Ray::EventListener<Ray::KeyPressedEvent>
{
public:
	App() { Ray::Scene::Create<Scene>(); }

private:
	void Update() override {}

	void OnEvent(Ray::MouseButtonPressedEvent& e) override
	{
		static std::mt19937 device;
		static std::uniform_real_distribution<float> posRotDist(-100.0f, 100.0f);
		static std::uniform_real_distribution<float> scaleDist(0.2f, 2.5f);
		static uint32_t modelCount = 0;

		for (uint32_t i = 0; i < 1; ++i)
		{
			Ray::Entity& meshEntity = Ray::Scene::Get().CreateEntity();
			// Ray::Material texturedMaterial({ 1.0f, 1.0f, 1.0f, 1.0f }, nullptr, 0.0f, 0.0f,
			//	Ray::MakeRef<Ray::Texture2D>("Resources/Textures/gridbase.png"), nullptr, true,
			//	nullptr);
			Ray::Material defaultMaterial(
				{ "Resources/Shaders/DefaultShader.vert", "Resources/Shaders/DefaultShader.frag" });

			Ray::Mesh& mesh = meshEntity.Emplace<Ray::Mesh>(Ray::Mesh::Circle(defaultMaterial));

			// auto& meshTransform = mesh.GetTransform();
			// meshTransform.Translation(
			//	{ posRotDist(device), posRotDist(device), posRotDist(device) });
			// meshTransform.Rotation({ posRotDist(device), posRotDist(device), posRotDist(device)
			// }); meshTransform.Scale({ scaleDist(device), scaleDist(device), scaleDist(device) });

			m_ModelEntity = &Ray::Scene::Get().CreateEntity();
			Ray::Mesh& model = m_ModelEntity->Emplace<Ray::Mesh>(
				Ray::Mesh::Import("Resources/Models/Nanosuit/nanosuit.obj"));
			// auto& modelTransform = model.GetTransform();
			// modelTransform.Translation = { posRotDist(device), posRotDist(device),
			//	posRotDist(device) };
			// modelTransform.Rotation = { posRotDist(device), posRotDist(device),
			//	posRotDist(device) };
			// modelTransform.Scale = { scaleDist(device), scaleDist(device), scaleDist(device) };

			++modelCount;
		}
	}


	void OnEvent(Ray::KeyPressedEvent& e) override
	{
		if (e.GetKey() == Ray::Key::Up)
		{
			auto& mesh = m_ModelEntity->Get<Ray::Mesh>();
			mesh.GetTransform().Translation(
				mesh.GetTransform().Translation() + Ray::Float3{ 0.0f, 1.0f, 0.0f });
		}
		if (e.GetKey() == Ray::Key::Down)
		{
			auto& mesh = m_ModelEntity->Get<Ray::Mesh>();
			mesh.GetTransform().Translation(
				mesh.GetTransform().Translation() + Ray::Float3{ 0.0f, -1.0f, 0.0f });
		}
	}

private:
	Ray::Entity* m_ModelEntity = nullptr;
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
