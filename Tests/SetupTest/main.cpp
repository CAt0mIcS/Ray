#include <Core/REngine.h>
#include <Utils/RLogger.h>
#include <Devices/RWindow.h>
#include <Graphics/RGraphics.h>

#include <UI/RButton.h>

#include <Geometry/RMesh.h>
#include <Graphics/Images/RTexture2D.h>
#include <Graphics/Renderers/RRenderer.h>
#include <Graphics/Renderers/RMeshRenderer.h>
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
	Scene() : Ray::Scene(Ray::MakeScope<Ray::Camera>())
	{
		Ray::UInt2 size = Ray::Window::Get().GetFramebufferSize();
		GetCamera().SetPosition(glm::vec3(0.0f, 0.0f, -2.5f));
		GetCamera().SetRotation(glm::vec3(0.0f));
		GetCamera().SetRotationSpeed(0.07f);
		GetCamera().SetPerspective(60.0f, (float)size.x / (float)size.y, 0.1f, 512.0f);
		GetCamera().SetMovementSpeed(3.0f);

		Ray::Renderer::Get().Emplace<Ray::MeshRenderer>();
	}
};


class App :
	public Ray::Engine,
	Ray::EventListener<Ray::MouseButtonPressedEvent>,
	Ray::EventListener<Ray::KeyPressedEvent>
{
public:
	App()
	{
		Ray::Scene::Create<Scene>();

		// Create UI
		Ray::Entity buttonEntity = Ray::Scene::Get().CreateEntity();
		Ray::Button& button = buttonEntity.Emplace<Ray::Button>(Ray::Float2{ 0.0f, 0.0f });
	}

private:
	void Update() override {}

	void OnEvent(Ray::MouseButtonPressedEvent& e) override
	{
		static std::mt19937 device;
		static std::uniform_real_distribution<float> posRotDist(-30.0f, 30.0f);
		static std::uniform_real_distribution<float> scaleDist(0.2f, 2.5f);
		static std::uniform_real_distribution<float> colorDist(0.0f, 1.0f);
		static Ray::Float3 posOffset{};
		static int run = 0;

		for (uint32_t i = 0; i < 1; ++i)
		{
			Ray::Entity meshEntity = Ray::Scene::Get().CreateEntity();
			// Ray::Material texturedMaterial({ 1.0f, 1.0f, 1.0f, 1.0f }, nullptr, 0.0f, 0.0f,
			//	Ray::MakeRef<Ray::Texture2D>("Resources/Textures/gridbase.png"), nullptr, true,
			//	nullptr);

			Ray::Material defaultMaterial{ Ray::Material::CullMode(VK_CULL_MODE_NONE),
				Ray::Material::Color({ colorDist(device), colorDist(device), colorDist(device) }) };

			Ray::Mesh& mesh = meshEntity.Emplace<Ray::Mesh>(Ray::Mesh::Triangle(defaultMaterial));

			auto& meshTransform = mesh.GetTransform();
			meshTransform.SetTranslation(
				{ posRotDist(device), posRotDist(device), posRotDist(device) });
			meshTransform.SetRotation(
				{ posRotDist(device), posRotDist(device), posRotDist(device) });
			meshTransform.SetScale({ scaleDist(device), scaleDist(device), scaleDist(device) });

			m_ModelEntities.emplace_back(Ray::Scene::Get().CreateEntity());
			Ray::Mesh& model = m_ModelEntities.back().Emplace<Ray::Mesh>(
				Ray::Mesh::Import("Resources/Models/Nanosuit/nanosuit.obj"));
			model.GetTransform().SetTranslation({ posOffset });

			// modelTransform.SetTranslation(
			//	{ posRotDist(device), posRotDist(device), posRotDist(device) });
			// modelTransform.SetRotation(
			//	{ posRotDist(device), posRotDist(device), posRotDist(device) });
			// modelTransform.SetScale({ scaleDist(device), scaleDist(device), scaleDist(device) });

			posOffset = PositioningScript(run, posOffset);
		}
	}

	Ray::Float3 PositioningScript(
		int& run, Ray::Float3 posOffset, uint32_t peopleInRow = 10, uint32_t incX = 8)
	{
		if (run % peopleInRow == 0 && run != 0)
		{
			posOffset.z += 6;
			posOffset.x = 0;
		}

		posOffset.x *= -1;
		if (run % 2 == 0)
		{
			if (posOffset.x >= 0)
				posOffset.x += incX;
			else
				posOffset.x -= incX;
		}
		++run;

		return posOffset;
	}

	void OnEvent(Ray::KeyPressedEvent& e) override
	{
		if (e.GetKey() == Ray::Key::Up)
		{
			for (Ray::Entity modelEntity : m_ModelEntities)
			{
				auto& model = modelEntity.Get<Ray::Mesh>();
				model.GetTransform().Translate(Ray::Float3{ 0.0f, 1.0f, 0.0f });
				// model.GetTransform().Rotate(Ray::Float3{ 1.0f, 0.0f, 1.0f });
			}
		}
		if (e.GetKey() == Ray::Key::Down)
		{
			for (Ray::Entity modelEntity : m_ModelEntities)
			{
				auto& model = modelEntity.Get<Ray::Mesh>();
				model.GetTransform().Translate(Ray::Float3{ 0.0f, -1.0f, 0.0f });
				// model.GetTransform().Rotate(Ray::Float3{ -1.0f, 0.0f, -1.0f });
			}
		}
	}

private:
	std::vector<Ray::Entity> m_ModelEntities;
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

		return App{}.Run();
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
