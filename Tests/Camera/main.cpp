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

#include <Ray/Graphics/Images/RTexture.h>
#include <Ray/Graphics/Images/RTextureCubemap.h>
#include <Ray/Graphics/Pipelines/RGraphicsPipeline.h>
#include <Ray/Utils/RException.h>
#include <Ray/Core/RDynamicVertex.h>
#include <Ray/Graphics/Pipelines/Shader/RShader.h>
#include <Ray/Shading/RMaterial.h>

#include <Ray/Scene/RScene.h>
#include <Ray/Scene/RCamera.h>

#include <signal.h>
#include <random>
#include <filesystem>

#include <Ray/UI/RImGui.h>
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

		// GetCamera().SetPerspective(60.0f, (float)size.x / (float)size.y, 0.1f, 512.0f);
		GetCamera().SetOrthographic(0.f, size.x, 0.f, size.y);

		GetCamera().SetMovementSpeed(3.0f);
	}
};


class App : public Ray::Engine
{
public:
	App()
	{
		Ray::Scene::Create<Scene>();
#include "../ImGuiWindows.inl"

		auto pipeline = Ray::GraphicsPipeline::Builder()
							.SetShader(Ray::Shader::AcquireSourceFile(
								{ "Tests/Camera/Shaders/Orthographic.vert",
									"Tests/Camera/Shaders/Orthographic.frag" }))
							.SetCullMode(VK_CULL_MODE_NONE)
							.Acquire();

		auto material = Ray::Material::Builder(pipeline)
							.Set("Model.scale", Ray::Float2{ 1.f })
							.Set("Model.translate", Ray::Float2{ 0.f })
							.Acquire();

		Ray::Entity entity = Scene::Get().CreateEntity();
		entity.Emplace<Ray::Mesh>(Ray::Mesh::Plane(material));
		entity.Emplace<Ray::MeshRenderer>(material);

		auto& cam = Scene::Get().GetCamera();
		auto& model = entity.Get<Ray::Transform>().AsMatrix();
		auto mvp = cam.ShaderData.Projection * cam.ShaderData.View * model;
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
