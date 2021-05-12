#include <Core/REngine.h>
#include <Utils/RLogger.h>
#include <Devices/RWindow.h>
#include <Graphics/RGraphics.h>

#include <UI/RButton.h>

#include <Components/RMesh.h>
#include <Components/RSkybox.h>

#include <Graphics/Images/RTexture2D.h>
#include <Graphics/Images/RTextureCubemap.h>
#include <Graphics/Pipelines/RGraphicsPipeline.h>
#include <Utils/RException.h>
#include <Core/RDynamicVertex.h>

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

				// Ray::Mesh& mesh = m_Entity.Get<Ray::Mesh>();

				// Ray::Float3& translation =
				//	const_cast<Ray::Float3&>(mesh.GetTransform().Translation());
				// Ray::Float3& rotation = const_cast<Ray::Float3&>(mesh.GetTransform().Rotation());
				// Ray::Float3& scale = const_cast<Ray::Float3&>(mesh.GetTransform().Scale());

				// Ray::ImGUI::Float3Widget("Translation", translation);
				// Ray::ImGUI::Float3Widget("Rotation", rotation);
				// Ray::ImGUI::Float3Widget("Scale", scale);
				// ImGui::Spacing();

				// mesh.GetTransform().RecalculateCachedMatrix();

				ImGui::End();
			}

			Ray::Shader shader;
			shader.CreateShaderModule("Resources/Shaders/Flat_Tex.vert",
				*Ray::ReadFile("Resources/Shaders/Flat_Tex.vert"), "", VK_SHADER_STAGE_VERTEX_BIT);
			shader.CreateShaderModule("Resources/Shaders/Flat_Tex.frag",
				*Ray::ReadFile("Resources/Shaders/Flat_Tex.frag"), "",
				VK_SHADER_STAGE_FRAGMENT_BIT);
			shader.CreateReflection();

			Ray::DynamicVertex vertex(shader);

			for (uint32_t i = 0; i < 10; ++i)
			{
				uint32_t vID = vertex.BeginVertex();
				vertex["inPos"] = Ray::Float3{ 1.0f + (float)i / 100.0f, 0.0f + (float)i / 100.0f,
					0.0f + (float)i / 100.0f };
				vertex["inTexCoord"] =
					Ray::Float2{ 1.0f + (float)i / 100.0f, 0.0f + (float)i / 100.0f };
			}
		});
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
