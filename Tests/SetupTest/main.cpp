#include <Core/REngine.h>
#include <Utils/RLogger.h>
#include <Devices/RWindow.h>
#include <Graphics/RGraphics.h>

#include <UI/RButton.h>

#include <Components/RMesh.h>
#include <Components/RSkybox.h>
#include <Components/RPointLight.h>

#include <Graphics/Images/RTexture2D.h>
#include <Graphics/Images/RTextureCubemap.h>
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

			bool oldDiffuseMap = m_NoDiffuseMap;
			bool oldSpecularMap = m_NoSpecularMap;
			bool oldNormalMap = m_NoNormalMap;

			bool inputDiffuse = !m_NoDiffuseMap;
			bool inputSpecular = !m_NoSpecularMap;
			bool inputNormal = !m_NoNormalMap;

			ImGui::Checkbox("DiffuseMap", &inputDiffuse);
			ImGui::Checkbox("SpecularMap", &inputSpecular);
			ImGui::Checkbox("NormalMap", &inputNormal);

			m_NoDiffuseMap = !inputDiffuse;
			m_NoSpecularMap = !inputSpecular;
			m_NoNormalMap = !inputNormal;

			if (m_NoDiffuseMap != oldDiffuseMap || m_NoSpecularMap != oldSpecularMap ||
				m_NoNormalMap != oldNormalMap)
				m_MapConfigChanged = true;

			ImGui::End();
		});

		Ray::ImGUI::Get().RegisterNewFrameFunction([&]() {
			{
				ImGui::Begin("Light");

				Ray::Mesh& mesh = m_LightEntity.Get<Ray::Mesh>();

				Ray::Float3& translation =
					const_cast<Ray::Float3&>(mesh.GetTransform().Translation());
				Ray::Float3& rotation = const_cast<Ray::Float3&>(mesh.GetTransform().Rotation());
				Ray::Float3& scale = const_cast<Ray::Float3&>(mesh.GetTransform().Scale());

				Ray::ImGUI::Float3Widget("Translation", translation);
				Ray::ImGUI::Float3Widget("Rotation", rotation);
				Ray::ImGUI::Float3Widget("Scale", scale);
				ImGui::Spacing();

				mesh.GetTransform().RecalculateCachedMatrix();

				ImGui::End();
			}
			{
				if (!m_ModelEntity)
					return;

				ImGui::Begin("BrickWall");

				Ray::Mesh& mesh = m_ModelEntity->Get<Ray::Mesh>();

				Ray::Float3& translation =
					const_cast<Ray::Float3&>(mesh.GetTransform().Translation());
				Ray::Float3& rotation = const_cast<Ray::Float3&>(mesh.GetTransform().Rotation());
				Ray::Float3& scale = const_cast<Ray::Float3&>(mesh.GetTransform().Scale());

				Ray::ImGUI::Float3Widget("Translation", translation);
				Ray::ImGUI::Float3Widget("Rotation", rotation);
				Ray::ImGUI::Float3Widget("Scale", scale);
				ImGui::Spacing();

				mesh.GetTransform().RecalculateCachedMatrix();

				ImGui::End();
			}
		});

		// Ray::Scene::Get().CreateEntity().Emplace<Ray::Skybox>(
		//	Ray::MakeRef<Ray::Texture2D>("Resources/Textures/EquirectangularWorldMap.jpg"));

		// RAY_TODO:
		// Ray::Scene::Get().CreateEntity().Emplace<Ray::Skybox>(
		//	Ray::MakeRef<Ray::TextureCubemap>("Resources/Textures/cubemap_space.ktx"));

		m_LightEntity = Ray::Scene::Get().CreateEntity();
		m_LightEntity
			.Emplace<Ray::Mesh>(Ray::Mesh::UVSphere(Ray::Material{
				Ray::Material::LightingTechnique(Ray::Material::LightingTechnique::Flat) }))
			.GetTransform()
			.SetScale(Ray::Float3{ 0.2f });

		m_LightEntity.Emplace<Ray::PointLight>();
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

				Ray::Model::Flags flags = Ray::Model::Unspecified;
				if (m_NoDiffuseMap)
					flags = flags | Ray::Model::NoDiffuseMap;
				if (m_NoSpecularMap)
					flags = flags | Ray::Model::NoSpecularMap;
				if (m_NoNormalMap)
					flags = flags | Ray::Model::NoNormalMap;

				m_ModelEntity = Ray::Scene::Get().CreateEntity();
				m_ModelEntity
					->Emplace<Ray::Mesh>(
						Ray::Mesh::Import("Resources/Models/BrickModel/BrickModel.obj", flags,
							std::nullopt, Ray::Material::CullMode(VK_CULL_MODE_NONE)))
					.GetTransform()
					.SetTranslation({ 0.0f, -5.0f, 0.0f });

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
	Ray::Entity m_LightEntity;
	bool m_RenderModel = true;
	bool m_NoSpecularMap = false;
	bool m_NoNormalMap = false;
	// Not working if true
	bool m_NoDiffuseMap = false;
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
