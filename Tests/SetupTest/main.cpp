#include <Ray/Core/REngine.h>
#include <Ray/Utils/RLogger.h>
#include <Ray/Devices/RWindow.h>
#include <Ray/Graphics/RGraphics.h>

#include <Ray/Components/RMesh.h>
#include <Ray/Components/RMeshRenderer.h>
#include <Ray/Components/RTransform.h>
#include <Ray/Components/RSkybox.h>
#include <Ray/Components/RModel.h>
#include <Ray/Components/RScriptableEntity.h>
#include <Ray/Components/RMeshContainer.h>

#include <Ray/Graphics/Images/RTexture.h>
#include <Ray/Graphics/Images/RTextureCubemap.h>
#include <Ray/Graphics/Pipelines/RGraphicsPipeline.h>
#include <Ray/Graphics/Pipelines/Shader/RShader.h>
#include <Ray/Utils/RException.h>
#include <Ray/Core/RDynamicVertex.h>

#include <Ray/Scene/RScene.h>
#include <Ray/Scene/RCamera.h>
#include <Ray/Shading/RMaterial.h>

#include <signal.h>
#include <random>
#include <filesystem>

#include <Ray/Utils/RImGui.h>
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
	}
};


class App : public Ray::Engine
{
public:
	App()
	{
		Ray::Scene::Create<Scene>();
		RAY_IMGUI(
			[&]()
			{
				{
					auto translate = [](Ray::Transform& tform)
					{
						Ray::Float3 newTranslation =
							Ray::ImGUI::Float3Widget("Translation", tform.Translation());
						Ray::Float3 newRotation =
							Ray::ImGUI::Float3Widget("Rotation", tform.Rotation());
						Ray::Float3 newScale = Ray::ImGUI::Float3Widget("Scale", tform.Scale());

						if (newTranslation != tform.Translation())
							tform.SetTranslation(newTranslation);
						if (newRotation != tform.Rotation())
							tform.SetRotation(newRotation);
						if (newScale != tform.Scale())
							tform.SetScale(newScale);
					};

					{
						ImGui::Begin("TestEntity");
						if (m_SelectedEntity)
						{
							Ray::Transform& tform = m_SelectedEntity.Get<Ray::Transform>();
							translate(tform);
							ImGui::Spacing();
						}


						// Scene Hierachy
						DrawEntityNode(m_Entity);

						ImGui::End();
					}
				}
			});

#include "../ImGuiWindows.inl"


		auto pipeline =
			Ray::GraphicsPipeline::Builder()
				.SetShader(Ray::Shader::AcquireSourceFile(
					{ "Resources/Shaders/Flat_Col.vert", "Resources/Shaders/Flat_Col.frag" }))
				.SetCullMode(VK_CULL_MODE_NONE)
				.Acquire();

		auto material =
			Ray::Material::Builder(pipeline).Set("Shading.color", Ray::Float4{ 1.f }).Build();

		m_Entity = Scene::Get().CreateEntity();
		// m_Entity.Emplace<Ray::Mesh>(Ray::Mesh::Import("Resources/Models/Nanosuit/nanosuit.obj"));
		m_Entity.Emplace<Ray::Model>("Resources/Scenes/ShadowMapping.glb", material);

		// m_Entity2 = Scene::Get().CreateEntity();
		// m_Entity2.Emplace<Ray::Mesh>(Ray::Mesh::Import("Resources/Models/Nanosuit/nanosuit.obj"));
		// m_Entity2.Get<Ray::Transform>().SetTranslation(Ray::Float3{ 6.0f, 0.0f, 0.0f });

		// Scene::Get().CreateEntity().Emplace<Ray::Skybox>(
		//	Ray::MakeRef<Ray::Texture>("Resources/Textures/EquirectangularWorldMap.jpg"));
		auto& registry = Scene::Get().GetRegistry();
	}

private:
	void Update() override {}

	void DrawEntityNode(Ray::Entity e)
	{
		std::string_view tag = "Empty";
		if (e.Has<Ray::Mesh>())
			tag = e.Get<Ray::Mesh>().GetName();
		else if (e.Has<Ray::MeshContainer>())
			tag = e.Get<Ray::MeshContainer>().GetName();

		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == e) ? ImGuiTreeNodeFlags_Selected : 0) |
								   ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)e, flags, tag.data());
		if (ImGui::IsItemClicked())
			m_SelectedEntity = e;

		if (opened)
		{
			for (Ray::Entity child : e.GetChildren())
				DrawEntityNode(child);
			ImGui::TreePop();
		}
	}

private:
	Ray::Entity m_Entity;
	Ray::Entity m_SelectedEntity;
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
