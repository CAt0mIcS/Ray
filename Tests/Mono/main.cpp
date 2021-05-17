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
		Ray::ImGUI::Get().RegisterNewFrameFunction([&]() {
			{
				ImGui::Begin("TestEntity");

				Ray::Transform& tform = m_Entity.Get<Ray::Transform>();

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
		Ray::ImGUI::Get().RegisterNewFrameFunction([&]() {
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

		m_Entity = Scene::Get().CreateEntity();
		auto& scriptableEntity =
			m_Entity.Emplace<Ray::ScriptableEntity>("Tests/Mono/CSharp/Example.cs", "TestScript");
		const auto& script = scriptableEntity.GetScript();
		const auto& object = scriptableEntity.GetObject();

		auto staticFunction = script.GetStaticFunction("TestScript:StaticMethod(int)");
		staticFunction(3);

		auto function = object.GetFunction("Update");
		function(12);

		m_Entity.Emplace<Ray::Mesh>(Ray::Mesh::Import("Resources/Models/Nanosuit/nanosuit.obj"));

		Scene::Get().CreateEntity().Emplace<Ray::Skybox>(
			Ray::MakeRef<Ray::Texture2D>("Resources/Textures/EquirectangularWorldMap.jpg"));
	}

private:
	void Update() override {}

private:
	Ray::Entity m_Entity;
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
