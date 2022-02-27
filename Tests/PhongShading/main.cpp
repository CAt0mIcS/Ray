#include <Ray/Core/REngine.h>
#include <Ray/Utils/RLogger.h>
#include <Ray/Devices/RWindow.h>
#include <Ray/Graphics/RGraphics.h>

#include <Ray/Components/RMesh.h>
#include <Ray/Components/RMeshRenderer.h>
#include <Ray/Components/RTransform.h>
#include <Ray/Components/RSkybox.h>
#include <Ray/Components/RScriptableEntity.h>

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


using namespace At0::Ray;


class Scene2 : public Scene
{
public:
	Scene2() : Scene(MakeScope<Camera>())
	{
		UInt2 size = Window::Get().GetFramebufferSize();
		GetCamera().SetPosition(Float3(0.0f, 0.0f, -2.5f));
		GetCamera().SetRotation(Float3(0.0f));
		GetCamera().SetRotationSpeed(0.07f);
		GetCamera().SetPerspective(60.0f, (float)size.x / (float)size.y, 0.1f, 512.0f);
		GetCamera().SetMovementSpeed(3.0f);
	}
};


class App : public Engine
{
public:
	App()
	{
		Scene::Create<Scene2>();
		ImGUI::Get().RegisterNewFrameFunction(
			[&]()
			{
				{
					ImGui::Begin("Light");

					Float3 lightPos = ImGUI::Float3Widget(
						"LightPos", m_Material->Get<Float3>("Shading.lightPosition"));
					m_Material->Set("Shading.lightPosition", lightPos);

					Float4 ambientLightColor = m_Material->Get<Float4>("Shading.ambientLightColor");

					ambientLightColor = Float4{ ImGUI::Float3Widget("AmbientLightColor",
													Float3{ ambientLightColor }),
						ambientLightColor.w };

					ImGui::SliderFloat("Intensity", &ambientLightColor.w, 0.001f, .5f);
					m_Material->Set("Shading.ambientLightColor", ambientLightColor);

					// RAY_TODO: Why is z direction reversed?
					m_Light.Get<Transform>().SetTranslation(
						Float3{ lightPos.x, lightPos.y, lightPos.z });

					ImGui::End();
				}
			});

		m_Entity = Scene::Get().CreateEntity();
		m_Entity.Emplace<Mesh>(Mesh::Import("Resources/Models/Plane.obj"));
		m_Entity.Get<Transform>().SetTranslation({ 0.f, .5f, 0.f }).SetScale(Float3{ 6.f });
		m_Material = m_Entity.Get<MeshRenderer>().GetSharedMaterial();

		Entity smoothVase = Scene::Get().CreateEntity();
		smoothVase.Emplace<Mesh>(Mesh::Import("Resources/Models/SmoothVase.obj", m_Material));
		smoothVase.Get<Transform>().SetScale(Float3{ 6.f });

		Entity flatVase = Scene::Get().CreateEntity();
		flatVase.Emplace<Mesh>(Mesh::Import("Resources/Models/FlatVase.obj", m_Material));
		flatVase.Get<Transform>().SetTranslation(Float3{ 4.f, 0.f, 0.f }).SetScale(Float3{ 6.f });

		auto flatColorPipeline =
			GraphicsPipeline::Builder()
				.SetShader(Shader::AcquireSourceFile(
					{ "Resources/Shaders/Flat_Col.vert", "Resources/Shaders/Flat_Col.frag" }))
				.Acquire();

		auto flatWhiteMaterial =
			Material::Builder(flatColorPipeline).Set("Shading.color", Float4{ 1.f }).Acquire();

		m_Light = Scene::Get().CreateEntity();
		m_Light.Emplace<Mesh>(Mesh::UVSphere(flatWhiteMaterial, .1f, 24, 24));
	}

private:
	void Update() override {}

private:
	Entity m_Entity;
	Entity m_Light;
	Ref<Material> m_Material;
};

void SignalHandler(int signal)
{
	Log::Critical("Signal {0} received", signal);
	Log::Close();
}

int main()
{
	signal(SIGABRT, SignalHandler);
	signal(SIGILL, SignalHandler);
	signal(SIGINT, SignalHandler);

	Log::Open("Ray.log");
	Log::SetLogLevel(At0::Violent::LogLevel::Information);

	try
	{
		Log::Info("Launch Path: \"{0}\"", std::filesystem::absolute("."));
		Window::Create();
		Window::Get().Show();
		Window::Get().SetTitle("SetupTest");

		return App{}.Run();
	}
	catch (Exception& e)
	{
		Log::Critical("{0}: {1}", e.GetType(), e.what());
	}
	catch (std::exception& e)
	{
		Log::Critical("Standard Exception: {0}", e.what());
	}
	catch (...)
	{
		Log::Critical("Unknown exception occured.");
	}
}
