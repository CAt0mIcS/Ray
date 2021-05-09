#include <Core/REngine.h>
#include <Utils/RLogger.h>
#include <Devices/RWindow.h>
#include <Graphics/RGraphics.h>

#include <UI/RButton.h>

#include <Components/RMesh.h>
#include <Components/RSkybox.h>

#include <Graphics/Images/RTexture2D.h>
#include <Graphics/Renderers/RRenderer.h>
#include <Graphics/Renderers/RMeshRenderer.h>
#include <Graphics/Pipelines/RGraphicsPipeline.h>
#include <Utils/RException.h>

#include <Graphics/RVertex.h>
#include <Scene/RScene.h>
#include <Scene/RCamera.h>

#include <signal.h>
#include <random>
#include <filesystem>

#include <UI/RImGui.h>
#include <../../Extern/imgui/imgui.h>


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

		Ray::Renderer::Get().Emplace<Ray::MeshRenderer>();
	}
};


class App : public Ray::Engine
{
public:
	App()
	{
		Ray::Scene::Create<Scene>();
		Ray::ImGUI::Get().RegisterNewFrameFunction([&]() {
			ImGui::Begin("Nanosuit");
			ImGui::Checkbox("RenderModel", &m_RenderModel);

			bool oldDiffuseMap = m_DiffuseMap;
			bool oldSpecularMap = m_SpecularMap;

			ImGui::Checkbox("DiffuseMap", &m_DiffuseMap);
			ImGui::Checkbox("SpecularMap", &m_SpecularMap);

			if (m_DiffuseMap != oldDiffuseMap || m_SpecularMap != oldSpecularMap)
				m_MapConfigChanged = true;

			ImGui::End();
		});

		Ray::Scene::Get().CreateEntity().Emplace<Ray::Skybox>(
			Ray::MakeRef<Ray::Texture2D>("Resources/Textures/EquirectangularWorldMap.jpg"));
	}

private:
	void Update() override
	{
		if (m_RenderModel)
		{
			if (m_MapConfigChanged || !m_ModelEntity)
			{
				if (m_ModelEntity)
					Ray::Scene::Get().DestroyEntity(*m_ModelEntity);

				Ray::Model::Flags flags = Ray::Model::NoNormalMap;
				if (m_DiffuseMap && m_SpecularMap)
				{
				}
				else if (m_DiffuseMap)
					flags = flags | Ray::Model::NoSpecularMap;
				else if (m_SpecularMap)
					flags = flags | Ray::Model::NoDiffuseMap;
				else
					flags = flags | Ray::Model::NoDiffuseMap | Ray::Model::NoSpecularMap;

				m_ModelEntity = Ray::Scene::Get().CreateEntity();
				m_ModelEntity->Emplace<Ray::Mesh>(
					Ray::Mesh::Import("Resources/Models/Nanosuit/nanosuit.obj", flags));
				m_MapConfigChanged = false;
			}
		}
		else if (!m_RenderModel && m_ModelEntity)
		{
			Ray::Scene::Get().DestroyEntity(*m_ModelEntity);
			m_ModelEntity = std::nullopt;
		}
	}

private:
	std::optional<Ray::Entity> m_ModelEntity;
	bool m_RenderModel = true;
	bool m_SpecularMap = true;
	// Not working if false
	bool m_DiffuseMap = true;
	bool m_MapConfigChanged = false;
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
#ifdef NDEBUG
	Ray::Log::SetLogLevel(Violent::LogLevel::Information);
#else
	Ray::Log::SetLogLevel(Violent::LogLevel::Information);
#endif

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
