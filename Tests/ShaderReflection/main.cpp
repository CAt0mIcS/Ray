#include <signal.h>

#include <Scene/RScene.h>
#include <Scene/REntity.h>
#include <Scene/RCamera.h>
#include <Core/REngine.h>
#include <Devices/RWindow.h>
#include <Utils/RException.h>
#include <Utils/RLogger.h>
#include <Graphics/Images/RTexture2D.h>

#include <Shading/Flat/RFlatColorMaterial.h>

#include <Components/RMesh.h>
#include <Components/RMeshRenderer.h>
#include <Components/RTransform.h>
#include <Components/RSkybox.h>

#include <UI/RImGui.h>
#include <../../Extern/imgui/imgui.h>
#include <Graphics/Pipelines/Shader/RShader.h>
#include <Graphics/Pipelines/Shader/RShaderReflection.h>
#include <Graphics/RCodex.h>


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


class CustomMaterial : public Ray::Material
{
public:
	CustomMaterial()
	{
		Ray::GraphicsPipeline::Layout pipelineLayout{};
		pipelineLayout.cullMode = VK_CULL_MODE_NONE;
		// pipelineLayout.shader = Ray::Shader::FromCompiled({ "", "" });
		pipelineLayout.shader = Ray::Shader::FromGlsl(
			{ "Resources/Shaders/Flat_Col.vert", "Resources/Shaders/Flat_Col.frag" });

		m_GraphicsPipeline = Ray::Codex::Resolve<Ray::GraphicsPipeline>(std::move(pipelineLayout));
	}

	virtual ~CustomMaterial() {}
};


class App : public Ray::Engine
{
public:
	App()
	{
		Ray::Scene::Create<Scene>();

#include "../ImGuiWindows.inl"

		// Ray::GraphicsPipeline::Layout layout{};
		// layout.cullMode = VK_CULL_MODE_NONE;
		// auto material = Ray::MakeRef<Ray::FlatColorMaterial>(
		//	Ray::FlatColorMaterial::Layout{}, std::move(layout));

		auto material = Ray::MakeRef<CustomMaterial>();

		m_Entity = Scene::Get().CreateEntity();
		m_Entity.Emplace<Ray::Mesh>(Ray::Mesh::Triangle(material));
		auto& renderer = m_Entity.Emplace<Ray::MeshRenderer>(material);
		renderer.GetBufferUniform("Shading")["color"] = Ray::Float4{ 1.0f };
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
	Ray::Log::SetLogLevel(Violent::LogLevel::Debug);

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
