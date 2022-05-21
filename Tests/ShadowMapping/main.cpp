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
#include <Ray/Components/RHierachyComponent.h>

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

#include <Ray/Graphics/RenderPass/RAttachment.h>
#include <Ray/Graphics/RenderPass/RRenderPass.h>
#include <Ray/Graphics/RenderPass/RSubpass.h>
#include <Ray/Graphics/Buffers/RFramebuffer.h>
#include <Ray/Graphics/Images/RImageView.h>
#include <Ray/Graphics/Images/RImage.h>
#include <Ray/Graphics/Pipelines/Shader/DataAccess/RDescriptor.h>
#include <Ray/Graphics/Pipelines/Shader/DataAccess/RBufferUniform.h>
#include <Ray/Graphics/Commands/RCommandBuffer.h>
#include <Ray/Graphics/Images/RImage.h>
#include <Ray/Graphics/Images/RTexture.h>
#include <Ray/Utils/RGeometricPrimitives.h>
#include <Ray/Graphics/Core/RPhysicalDevice.h>
#include <Ray/Graphics/Core/RSwapchain.h>
#include <Ray/Graphics/ShadowMapping/RShadowMappingObjects.h>


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

		{
			auto shadedPipeline =
				GraphicsPipeline::Builder()
					.SetShader(Shader::AcquireSourceFile({ "Resources/Shaders/ShadowMapping.vert",
						"Resources/Shaders/ShadowMapping.frag" }))
					.SetCullMode(VK_CULL_MODE_NONE)
					.Acquire();

			m_Material = Material::Builder(shadedPipeline).Build();

			m_Floor = Scene::Get().CreateEntity();
			m_Floor.Emplace<Model>("Resources/Models/Plane.obj", m_Material);
			m_Floor.Get<Mesh>().SetName("Floor");
			m_Floor.Get<Transform>().SetScale(Float3{ 6.f });

			m_Scene = Scene::Get().CreateEntity();
			m_Scene.Emplace<Model>("Resources/Models/SmoothVase.obj", m_Material);
			m_Scene.Get<Transform>()
				.SetTranslation(Float3{ 0.f, .4f, 0.f })
				.SetScale(Float3{ 2.f })
				.SetRotation(Float3{ Math::PI<>, 0.f, 0.f });

			auto flatColorPipeline =
				GraphicsPipeline::Builder()
					.SetShader(Shader::AcquireSourceFile(
						{ "Resources/Shaders/Flat_Col.vert", "Resources/Shaders/Flat_Col.frag" }))
					.Acquire();

			auto flatWhiteMaterial =
				Material::Builder(flatColorPipeline).Set("Shading.color", Float4{ 1.f }).Acquire();

			m_Light = Scene::Get().CreateEntity();
			m_Light.Emplace<PointLight>().SetTranslation({ 5.f, 5.f, 0.f });
		}


		const auto& shadowMapping = Graphics::Get().GetShadowMapping();

		m_Material->Set(
			"Shading.lightSpace", shadowMapping.CalculateDepthModelViewProjectionMatrix());
		m_Material->Set("shadowMap", shadowMapping.framebufferImage,
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
	}

private:
	void Update() override
	{
		Matrix rotateLight =
			glm::rotate(Matrix{ 1.f }, GetDelta().AsSeconds(), { 0.f, -5.5f, 0.f });
		m_Light.Get<PointLight>().SetTranslation(
			rotateLight * Float4{ m_Light.Get<Transform>().Translation(), 1.f });

		m_Material->Set("Shading.lightSpace",
			Graphics::Get().GetShadowMapping().CalculateDepthModelViewProjectionMatrix());
		m_Material->Set(
			"Shading.lightPosition", Float4{ m_Light.Get<Transform>().Translation(), 1.f });
	}

private:
	Entity m_Scene;
	Entity m_Floor;
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
