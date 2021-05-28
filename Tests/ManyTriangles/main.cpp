#include <Core/REngine.h>
#include <Utils/RLogger.h>
#include <Devices/RWindow.h>
#include <Graphics/RGraphics.h>

#include <UI/RButton.h>

#include <Components/RMesh.h>
#include <Components/RMeshRenderer.h>
#include <Components/RTransform.h>
#include <Components/RSkybox.h>
#include <Components/RScriptableEntity.h>
#include <Components/RTagComponent.h>

#include <Graphics/Images/RTexture2D.h>
#include <Graphics/Images/RTextureCubemap.h>
#include <Graphics/Pipelines/RGraphicsPipeline.h>
#include <Utils/RException.h>
#include <Core/RDynamicVertex.h>

#include <Shading/Phong/RPhongMaterial.h>
#include <Shading/Flat/RFlatColorMaterial.h>

#include <Scene/RScene.h>
#include <Scene/RCamera.h>

#include <signal.h>
#include <random>
#include <filesystem>

#include <UI/RImGui.h>
#include <../../Extern/imgui/imgui.h>

#include <Mono/RScript.h>


using namespace At0;


class Scene : public Ray::Scene
{
public:
	Scene() : Ray::Scene(Ray::MakeScope<Ray::Camera>())
	{
		Ray::UInt2 size = Ray::Window::Get().GetFramebufferSize();
		GetCamera().SetPosition(Ray::Float3(0.0f, 0.0f, -2.5f));
		GetCamera().SetRotation(Ray::Float3(0.0f));
		GetCamera().SetRotationSpeed(0.07f);
		GetCamera().SetPerspective(60.0f, (float)size.x / (float)size.y, 0.1f, 512.0f);
		GetCamera().SetMovementSpeed(3.0f);
	}
};


class App : public Ray::Engine
{
public:
	App()
	{
		Ray::Scene::Create<Scene>();
		Scene::Get().CreateEntity().Emplace<Ray::Skybox>(
			Ray::Texture2D::Acquire("Resources/Textures/EquirectangularWorldMap.jpg"));

		Ray::ImGUI::Get().RegisterNewFrameFunction([this]() {
			ImGui::Begin("Triangles");
			ImGui::InputInt("Upper range", &m_UpperRange, 100, 10000);
			int prevTriangleCount = m_Triangles;
			ImGui::SliderInt("Triangle Count", &m_Triangles, 0, m_UpperRange);
			if (prevTriangleCount != m_Triangles)
				TriangleCountChanged(prevTriangleCount);
			ImGui::End();
		});


		for (uint32_t i = 0; i < m_Triangles; ++i)
		{
			AddTriangle(std::to_string(i));
		}
	}

private:
	void Update() override {}

	void AddTriangle(const std::string& tag)
	{
		static std::mt19937 device;
		static std::uniform_real_distribution<float> posRotDist(-30.0f, 30.0f);
		static std::uniform_real_distribution<float> scaleDist(0.2f, 2.5f);
		static std::uniform_real_distribution<float> colDist(0.0f, 1.0f);

		Ray::GraphicsPipeline::Layout layout{};
		layout.cullMode = VK_CULL_MODE_NONE;
		auto defaultMaterial = Ray::MakeRef<Ray::FlatColorMaterial>(
			Ray::FlatColorMaterial::Layout{}, std::move(layout));

		Ray::Entity entity = Scene::Get().CreateEntity();
		entity.Emplace<Ray::TagComponent>("Triangle_" + tag);
		Ray::Mesh& mesh = entity.Emplace<Ray::Mesh>(Ray::Mesh::Triangle(defaultMaterial));
		auto& renderer = entity.Emplace<Ray::MeshRenderer>(defaultMaterial);
		renderer.GetBufferUniform("Shading")["color"] =
			Ray::Float3{ colDist(device), colDist(device), colDist(device) };

		auto& transform = entity.Get<Ray::Transform>();
		transform.SetTranslation({ posRotDist(device), posRotDist(device), posRotDist(device) });
		transform.SetRotation({ posRotDist(device), posRotDist(device), posRotDist(device) });
		transform.SetScale({ scaleDist(device), scaleDist(device), scaleDist(device) });
	}

	void RemoveTriangle()
	{
		auto triangleView = Scene::Get().EntityView<Ray::TagComponent>();
		Scene::Get().DestroyEntity(triangleView.back());
	}

	void TriangleCountChanged(int prevTriangleCount)
	{
		if (m_Triangles > prevTriangleCount)
		{
			for (uint32_t i = prevTriangleCount; i < m_Triangles; ++i)
				AddTriangle(std::to_string(i));
		}
		else if (m_Triangles < prevTriangleCount)
		{
			for (uint32_t i = prevTriangleCount; i > m_Triangles; --i)
				RemoveTriangle();
		}
	}

private:
	int m_Triangles = 200;
	int m_UpperRange = 10000;
	// int m_Triangles = 5460;
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
		Ray::Log::Info("Launch Path: \"{0}\"", std::filesystem::absolute("."));
		Ray::Window::Create();
		Ray::Window::Get().Show();
		Ray::Window::Get().SetTitle("SetupTest");

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
