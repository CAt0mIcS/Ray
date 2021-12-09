#include <Ray/Core/REngine.h>
#include <Ray/Utils/RLogger.h>
#include <Ray/Devices/RWindow.h>
#include <Ray/Graphics/RGraphics.h>

#include <Ray/UI/RButton.h>

#include <Ray/Components/RMesh.h>
#include <Ray/Components/RMeshRenderer.h>
#include <Ray/Components/RTransform.h>
#include <Ray/Components/RSkybox.h>
#include <Ray/Components/RScriptableEntity.h>
#include <Ray/Components/RTagComponent.h>

#include <Ray/Graphics/Images/RTexture2D.h>
#include <Ray/Graphics/Images/RTextureCubemap.h>
#include <Ray/Graphics/Pipelines/RGraphicsPipeline.h>
#include <Ray/Utils/RException.h>
#include <Ray/Graphics/Pipelines/Shader/RShader.h>
#include <Ray/Core/RDynamicVertex.h>

#include <Ray/Scene/RScene.h>
#include <Ray/Scene/RCamera.h>

#include <signal.h>
#include <random>
#include <filesystem>

#include <Ray/UI/RImGui.h>
#include <../../Extern/imgui/imgui.h>

#include <Ray/Mono/RScript.h>


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

		Ray::ImGUI::Get().RegisterNewFrameFunction(
			[this]()
			{
				{
					ImGui::Begin("Skybox");
					static bool enabled = false;
					bool previous = enabled;
					ImGui::Checkbox("Enabled", &enabled);

					if (previous != enabled)
					{
						if (enabled)
							Scene::Get().CreateEntity().Emplace<Ray::Skybox>(
								Ray::Texture2D::Acquire(
									"Resources/Textures/EquirectangularWorldMap.jpg"));
						else
							Scene::Get().DestroyEntity(Scene::Get().EntityView<Ray::Skybox>()[0]);
					}
					ImGui::End();
				}

				{
					ImGui::Begin("Triangles");
					ImGui::InputInt("Upper range", &m_UpperRange, 100, 10000);
					int prevTriangleCount = m_Triangles;
					ImGui::SliderInt("Triangle Count", &m_Triangles, 0, m_UpperRange);
					if (prevTriangleCount != m_Triangles)
						TriangleCountChanged(prevTriangleCount);
					ImGui::SliderFloat("Movement Speed", &m_MovementSpeed, -50.0f, 50.0f);
					ImGui::End();
				}

				{
					ImGui::Begin("Color");
					Ray::Float3 oldColor = m_Material->Get<Ray::Float3>("Shading.color");
					Ray::Float3 newColor = Ray::ImGUI::Float3Widget("Color", oldColor);
					if (oldColor != newColor)
						m_Material->Set("Shading.color", Ray::Float4{ newColor, 1.0f });
					ImGui::End();
				}
			});

#include "../ImGuiWindows.inl"


		auto defaultPipeline =
			Ray::GraphicsPipeline::Builder()
				.SetCullMode(VK_CULL_MODE_NONE)
				.SetShader(Ray::Shader::Acquire(
					{ "Resources/Shaders/Flat_Col.vert", "Resources/Shaders/Flat_Col.frag" }))
				.Acquire();

		std::mt19937 device;
		std::uniform_real_distribution<float> colDist(0.0f, 1.0f);

		m_Material = Ray::Material::Builder(defaultPipeline)
						 .Set("Shading.color",
							 Ray::Float4{ colDist(device), colDist(device), colDist(device), 1.0f })
						 .Acquire();


		for (uint32_t i = 0; i < m_Triangles; ++i)
		{
			AddTriangle(std::to_string(i));
		}
	}

private:
	void Update() override
	{
		if (m_MovementSpeed == 0.0f)
			return;

		auto view = Scene::Get().EntityView<Ray::TagComponent>();
		for (Ray::Entity e : view)
			e.Get<Ray::Transform>().Translate(
				Ray::Float3{ m_MovementSpeed * Ray::Engine::Get().GetDelta().AsSeconds() });
	}

	void AddTriangle(const std::string& tag)
	{
		static std::mt19937 device;
		static std::uniform_real_distribution<float> posRotDist(-100.0f, 100.0f);
		static std::uniform_real_distribution<float> scaleDist(0.2f, 2.5f);

		Ray::Entity entity = Scene::Get().CreateEntity();
		entity.Emplace<Ray::TagComponent>("Triangle_" + tag);
		Ray::Mesh& mesh = entity.Emplace<Ray::Mesh>(Ray::Mesh::Triangle(m_Material));
		auto& renderer = entity.Emplace<Ray::MeshRenderer>(m_Material);

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
	int m_Triangles = 25;
	int m_UpperRange = 100000;
	float m_MovementSpeed = 0.0f;

	Ray::Ref<Ray::Material> m_Material;
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
	Ray::Log::SetLogLevel(Violent::LogLevel::Trace);

	try
	{
		Ray::UniformBuffer::SetAllocationSize(209715200);

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
