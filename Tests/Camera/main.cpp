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
#include <Ray/Graphics/Buffers/RVertexBuffer.h>
#include <Ray/Graphics/Buffers/RIndexBuffer.h>

#include <Ray/Graphics/Images/RTexture.h>
#include <Ray/Graphics/Images/RTextureCubemap.h>
#include <Ray/Graphics/Pipelines/RGraphicsPipeline.h>
#include <Ray/Utils/RException.h>
#include <Ray/Core/RDynamicVertex.h>
#include <Ray/Graphics/Pipelines/Shader/RShader.h>
#include <Ray/Shading/RMaterial.h>
#include <Ray/Graphics/RGraphics.h>
#include <Ray/Graphics/Core/RSwapchain.h>

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
		VkExtent2D size = Ray::Graphics::Get().GetSwapchain().GetExtent();
		GetCamera().SetPosition(Ray::Float3(0.0f, 0.0f, -2.5f));
		GetCamera().SetRotation(Ray::Float3(0.0f));
		GetCamera().SetRotationSpeed(0.07f);

		// GetCamera().SetPerspective(60.0f, (float)size.x / (float)size.y, 0.1f, 512.0f);
		// GetCamera().SetOrthographic(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f);
		GetCamera().SetOrthographic(0.f, size.width, 0.f, size.height, -1.f, 1.f);

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

		auto material = Ray::Material::Builder(std::move(pipeline)).Acquire();

		Ray::Entity entity = Scene::Get().CreateEntity();
		entity.Emplace<Ray::Mesh>(GetPlane(material));
	}

private:
	Ray::Mesh::Data GetPlane(Ray::Ref<Ray::Material> material)
	{
		using namespace Ray;
		DynamicVertex vertex(material->GetGraphicsPipeline().GetShader());

#define SMALL_VERTICES 0

#if SMALL_VERTICES
		vertex.BeginVertex();
		vertex[AttributeMap<AttributeType::Position2D>::Semantic] = Float2{ -0.5f, -0.5f };
		vertex.BeginVertex();
		vertex[AttributeMap<AttributeType::Position2D>::Semantic] = Float2{ 0.5f, -0.5f };
		vertex.BeginVertex();
		vertex[AttributeMap<AttributeType::Position2D>::Semantic] = Float2{ 0.5f, 0.5f };
		vertex.BeginVertex();
		vertex[AttributeMap<AttributeType::Position2D>::Semantic] = Float2{ -0.5f, 0.5f };
#else
		vertex.BeginVertex();
		vertex[AttributeMap<AttributeType::Position2D>::Semantic] = Float2{ 240.f, 135.f };
		vertex.BeginVertex();
		vertex[AttributeMap<AttributeType::Position2D>::Semantic] = Float2{ 720.f, 135.f };
		vertex.BeginVertex();
		vertex[AttributeMap<AttributeType::Position2D>::Semantic] = Float2{ 720.f, 405.f };
		vertex.BeginVertex();
		vertex[AttributeMap<AttributeType::Position2D>::Semantic] = Float2{ 240.f, 405.f };
#endif

		std::vector<IndexBuffer::Type> indices{ 0, 1, 2, 2, 3, 0 };
		return { MakeRef<VertexBuffer>("VtxCameraTest", std::move(vertex)),
			MakeRef<IndexBuffer>("IdxCameraTest", std::move(indices)), std::move(material) };
	}

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
