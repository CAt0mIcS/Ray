#include <Ray/Core/REngine.h>
#include <Ray/Utils/RLogger.h>
#include <Ray/Devices/RWindow.h>
#include <Ray/Graphics/RGraphics.h>

#include <Ray/Components/RMesh.h>
#include <Ray/Components/RMeshRenderingResources.h>
#include <Ray/Components/RTransform.h>
#include <Ray/Components/RSkybox.h>
#include <Ray/Components/RModel.h>
#include <Ray/Components/RScriptableEntity.h>
#include <Ray/Components/RHierachyComponent.h>
#include <Ray/Components/RPointLight.h>
#include <Ray/Components/RTagComponent.h>

#include <Ray/Graphics/Images/RTexture.h>
#include <Ray/Graphics/Images/RTextureCubemap.h>
#include <Ray/Graphics/Pipelines/RGraphicsPipeline.h>
#include <Ray/Graphics/Pipelines/Shader/RShader.h>
#include <Ray/Utils/RException.h>
#include <Ray/Core/RDynamicVertex.h>

#include <Ray/Scene/RScene.h>
#include <Ray/Scene/RCamera.h>
#include <Ray/Shading/RMaterial.h>

#include <Ray/Layers/RLayer.h>

#include <signal.h>
#include <random>
#include <filesystem>

#include <Ray/Utils/RImGui.h>
#include <../../Extern/ImGui/ImGui.h>


using namespace At0;


class TestEntityLayer : public Ray::Layer, public Ray::EventListener<Ray::ImGuiDrawEvent>
{
public:
	TestEntityLayer(Ray::Scene& scene, Ray::Ref<Ray::Window> window) : Ray::Layer(scene, window)
	{
#if RAY_ENABLE_IMGUI
		RegisterForDispatcher(window->GetImGui());
#endif

		auto pipeline =
			PipelineBuilder()
				.SetShader(LoadShaderFromSourceFile({ "Tests/Lighting/Shaders/Lighting.vert",
					"Tests/Lighting/Shaders/Lighting.frag" }))
				.SetCullMode(VK_CULL_MODE_NONE)
				.Acquire();

		auto material = MaterialBuilder(pipeline)
							.Set("Shading.color", Ray::Float4{ 1.f })
							.Set("Shading.ambientLightColor", Ray::Float4{ 1.f, 1.f, 1.f, .1f })
							.Build();


		m_Entity = GetScene().CreateEntity();
		m_Entity.Emplace<Ray::Model>(scene, "Resources/Scenes/Sponza/scene.gltf", material);

		Ray::Entity light = GetScene().CreateEntity();
		light.Emplace<Ray::PointLight>();
		light.Emplace<Ray::TagComponent>("Light");
		light.Emplace<Ray::Mesh>(Ray::Mesh::UVSphere(Ray::Material::FlatWhite(), 1.f, 24, 24));

		m_Entity.AddChild(light);

		// GetScene().CreateEntity().Emplace<Ray::Skybox>(
		//	Ray::MakeRef<Ray::Texture>("Resources/Textures/EquirectangularWorldMap.jpg"));
		auto& registry = GetScene().GetRegistry();
	}

private:
	void DrawEntityNode(Ray::Entity e)
	{
		std::string_view tag = "Empty";
		if (e.Has<Ray::TagComponent>())
			tag = e.Get<Ray::TagComponent>().Tag;
		else if (e.Has<Ray::Mesh>())
			tag = e.Get<Ray::Mesh>().GetName();

		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == e) ? ImGuiTreeNodeFlags_Selected : 0) |
								   ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)e, flags, tag.data());
		if (ImGui::IsItemClicked())
			m_SelectedEntity = e;

		if (opened)
		{
			if (e.Has<Ray::HierachyComponent>())
				for (Ray::Entity child : e.GetChildren())
					DrawEntityNode(child);
			ImGui::TreePop();
		}
	}

	void OnEvent(Ray::ImGuiDrawEvent& e)
	{
#if RAY_ENABLE_IMGUI
		auto translate = [](Ray::Entity e)
		{
			auto& tform = e.Get<Ray::Transform>();

			Ray::Float3 newTranslation =
				Ray::ImGUI::Float3Widget("Translation", tform.Translation());
			Ray::Float3 newRotation = Ray::ImGUI::Float3Widget("Rotation", tform.Rotation());
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
				translate(m_SelectedEntity);
				ImGui::Spacing();
			}


			// Scene Hierachy
			DrawEntityNode(m_Entity);

			ImGui::End();
		}
#endif

#include "../ImGuiWindows.inl"
	}

private:
	Ray::Entity m_Entity;
	Ray::Entity m_SelectedEntity;
};


class App : public Ray::Engine
{
public:
	App()
	{
		auto mainWindow = CreateWindow("MainWindow");
		mainWindow->Show();
		mainWindow->SetTitle("SetupTest");

		// Ray::ImGui::Create(*mainWindow);

		// Ray::Ref<Ray::Scene> scene = CreateSceneFromFile("filepath");
		Ray::Ref<Ray::Scene> scene = CreateScene("MainScene");
		mainWindow->SetActiveScene(scene);

		Ray::UInt2 size = mainWindow->GetFramebufferSize();
		auto& camera = scene->GetCamera();
		camera.SetPosition(Ray::Float3(0.0f, 0.0f, -2.5f));
		camera.SetRotation(Ray::Float3(0.0f));
		camera.SetRotationSpeed(0.07f);
		camera.SetPerspective(60.0f, (float)size.x / (float)size.y, 0.1f, 512.0f);
		camera.SetMovementSpeed(3.0f);

		// RAY_TODO: Currently we need the Window (it's Vulkan Resources) in the Layer to be able to
		// e.g. create GraphicsPipeline::Builder
		//	Maybe decouple this in the future:
		//		Layer class in the Scene is only for game logic (updating entities, applying physics
		//			forces) (Then again, we have ScriptableEntity[Component] for game logic)
		//		RenderLayer class (stored in the Window) is for interacting with rendering resources
		//			(creating new graphics pipelines)
		//
		// So we'd call:
		//	scene->RegisterLayer<PhysicsLayer>();
		//	window->RegisterRenderingLayer<UiRenderingLayer>();
		scene->RegisterLayer<TestEntityLayer>(mainWindow);
	}

private:
	void Update() override {}
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
	Ray::Log::SetLogLevel(Violent::LogLevel::Debug);
#endif

	try
	{
		Ray::Log::Info("Launch Path: \"{0}\"", std::filesystem::absolute("."));
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
