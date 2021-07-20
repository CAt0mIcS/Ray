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

#include <Ray/Core/RRendererLoader.h>
#include <Ray/Graphics/Images/RTexture2D.h>
#include <Ray/Graphics/Images/RTextureCubemap.h>
#include <Ray/Graphics/Pipelines/RGraphicsPipeline.h>
#include <Ray/Utils/RException.h>
#include <Ray/Core/RDynamicVertex.h>

#include <Ray/Shading/Phong/RPhongMaterial.h>
#include <Ray/Shading/Flat/RFlatColorMaterial.h>

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


class App : public Ray::Engine, Ray::EventListener<Ray::MouseButtonPressedEvent>
{
public:
	App()
	{
		Ray::Scene::Create<Scene>();
		RAY_IMGUI_ONFRAME([&]() {
			{
				ImGui::Begin("TestEntity");

				Ray::Transform& tform = m_Entities[0].Get<Ray::Transform>();

				Ray::Float3& translation = const_cast<Ray::Float3&>(tform.Translation());
				Ray::Float3& rotation = const_cast<Ray::Float3&>(tform.Rotation());
				Ray::Float3& scale = const_cast<Ray::Float3&>(tform.Scale());

				Ray::ImGUI::Float3Widget("Translation", translation);
				Ray::ImGUI::Float3Widget("Rotation", rotation);
				Ray::ImGUI::Float3Widget("Scale", scale);
				ImGui::Spacing();

				tform.RecalculateCachedMatrix();

				ImGui::End();
			}
		});
		RAY_IMGUI_ONFRAME([&]() {
			{
				ImGui::Begin("Camera");

				ImGui::SliderFloat(
					"Rotation Speed", &Scene::Get().GetCamera().RotationSpeed, 0.0f, 100.0f);
				ImGui::SliderFloat(
					"Movement Speed", &Scene::Get().GetCamera().MovementSpeed, 0.1f, 100.0f);

				Ray::ImGUI::Float3Widget("Camera Position", Scene::Get().GetCamera().Position);
				Ray::ImGUI::Float3Widget("Camera Rotation", Scene::Get().GetCamera().Rotation);

				ImGui::End();
			}
		});
#include "../ImGuiWindows.inl"

		// RAY_TODO: Fix matrix not being recalculated at the beginning
		// auto sharedMaterial = Ray::MakeRef<Ray::FlatColorMaterial>();

		// for (uint32_t i = 0; i < 9; ++i)
		//{
		//	m_Entity = Scene::Get().CreateEntity();
		//	m_Entity.Emplace<Ray::Mesh>(Ray::Mesh::Plane(sharedMaterial));
		//	auto& renderer = m_Entity.Emplace<Ray::MeshRenderer>(sharedMaterial);
		//	renderer.GetBufferUniform("Shading")["color"] =
		//		Ray::Float3{ i / 3.0f, i / 4.0f, i / 2.0f };

		//	m_Entity.Get<Ray::Transform>().SetTranslation({ i + 1, 0.0f, 0.0f });
		//}

		auto e = m_Entities.emplace_back(Scene::Get().CreateEntity());
		auto& scriptableEntity =
			e.Emplace<Ray::ScriptableEntity>("Resources/Scripts/Example.cs", "TestScript");
		Ray::Mono::Script& script = scriptableEntity.GetScript();
		Ray::Mono::Object& object = scriptableEntity.GetObject();

		m_Update = object.GetFunction("Update");

		e.Emplace<Ray::Mesh>(Ray::Mesh::Import("Resources/Models/Nanosuit/nanosuit.obj"));

		Scene::Get().CreateEntity().Emplace<Ray::Skybox>(
			Ray::MakeRef<Ray::Texture2D>("Resources/Textures/EquirectangularWorldMap.jpg"));
	}

private:
	void Update() override
	{
		m_Update(Ray::Engine::Get().GetDelta().AsSeconds());

		Ray::Float3 translation = m_Entities[0]
									  .Get<Ray::ScriptableEntity>()
									  .GetObject()["translation"]
									  .Get<Ray::Float3>();
		for (uint32_t i = 0; i < m_Entities.size(); ++i)
		{
			m_Entities[i].Get<Ray::Transform>().SetTranslation(translation + (float)i);
		}
	}

	void OnEvent(Ray::MouseButtonPressedEvent& e)
	{
		if (e.GetKey() == Ray::MouseButton::Middle)
			m_Entities.emplace_back(Scene::Get().CreateEntity())
				.Emplace<Ray::Mesh>(Ray::Mesh::Import("Resources/Models/Nanosuit/nanosuit.obj"));
	}

private:
	std::vector<Ray::Entity> m_Entities;
	Ray::Mono::Function m_Update;
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
	Ray::Log::SetLogLevel(Violent::LogLevel::Debug);

	try
	{
		Ray::LoadRenderer(Ray::RendererAPI::Vulkan);

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
