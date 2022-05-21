#include <Ray/Core/REngine.h>
#include <Ray/Utils/RLogger.h>
#include <Ray/Devices/RWindow.h>
#include <Ray/Graphics/RGraphics.h>

#include <Ray/Components/RMesh.h>
#include <Ray/Components/RMeshRenderer.h>
#include <Ray/Components/RTransform.h>
#include <Ray/Components/RSkybox.h>
#include <Ray/Components/RScriptableEntity.h>
#include <Ray/Components/RPointLight.h>
#include <Ray/Components/RModel.h>

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

#include "../ImGuiWindows.inl"

		RAY_IMGUI(
			[this]()
			{
				ImGui::Begin("PointLight");

				PointLight& ptLight = m_PointLight.Get<PointLight>();

				Float3 oldTranslation = m_PointLight.Get<Transform>().Translation();
				Float3 newTranslation = ImGUI::Float3Widget("Translation", oldTranslation);
				if (oldTranslation != newTranslation)
					ptLight.SetTranslation(newTranslation);

				Float4 oldColor = ptLight.GetColor();
				Float4 newColor = ImGUI::Float4Widget("Color", oldColor);
				if (oldColor != newColor)
					ptLight.SetColor(newColor);

				ImGui::End();
			});

		auto pipeline =
			GraphicsPipeline::Builder()
				.SetShader(Shader::AcquireSourceFile(
					{ "Resources/Shaders/Phong_All.vert", "Resources/Shaders/Phong_All.frag" }))
				.Acquire();

		auto material = Material::Builder(pipeline)
							.Set("Shading.color", Float4{ 1.f })
							.Set("Shading.ambientLightColor", Float4{ 1.f, 1.f, 1.f, 0.05f })
							.Build();

		auto flatPipeline =
			GraphicsPipeline::Builder()
				.SetShader(Shader::AcquireSourceFile(
					{ "Resources/Shaders/Flat_Col.vert", "Resources/Shaders/Flat_Col.frag" }))
				.Acquire();

		auto flatMaterial =
			Material::Builder(flatPipeline).Set("Shading.color", Float4{ 1.f }).Build();

		m_Scene = Scene::Get().CreateEntity();
		m_Scene.Emplace<Model>("Resources/Scenes/Sponza/scene.gltf", material);

		m_PointLight = Scene::Get().CreateEntity();
		m_PointLight.Emplace<PointLight>();
		m_PointLight.Emplace<Mesh>(Mesh::UVSphere(flatMaterial, 0.1f, 24, 24));
	}

private:
	Entity m_Scene;

	Entity m_DirectionalLight;
	Entity m_PointLight;
	Entity m_SpotLight;
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
