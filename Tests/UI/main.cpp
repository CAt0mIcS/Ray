#define RAY_ENABLE_IMGUI 0

#include <Ray/Core/REngine.h>
#include <Ray/Utils/RLogger.h>
#include <Ray/Devices/RWindow.h>
#include <Ray/Graphics/RGraphics.h>

#include <Ray/Components/RMesh.h>
#include <Ray/Components/RTextRenderer.h>
#include <Ray/Components/RSkybox.h>
#include <Ray/Components/RTransform.h>

#include <Ray/Graphics/Images/RTexture2D.h>
#include <Ray/Graphics/Pipelines/RGraphicsPipeline.h>
#include <Ray/Utils/RException.h>
#include <Ray/Graphics/Pipelines/RGraphicsPipeline.h>
#include <Ray/Shading/RMaterial.h>
#include <Ray/Graphics/Pipelines/Shader/RShader.h>
#include <Ray/Components/RMeshRenderer.h>

#include <Ray/Scene/RScene.h>
#include <Ray/Scene/RCamera.h>

#include <signal.h>
#include <random>

#include <Ray/Events/REventListener.h>
#include <Ray/Events/REngineEvents.h>
#include <Ray/Events/RKeyboardEvents.h>
#include <Ray/Events/RMouseEvents.h>

#include <Ray/UI/RImGui.h>
#include <Ray/Graphics/Text/RFont.h>
#include <Ray/UI/RButton.h>

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


class App :
	public Ray::Engine,
	Ray::EventListener<Ray::HoverEnterEvent>,
	Ray::EventListener<Ray::HoverLeaveEvent>,
	Ray::EventListener<Ray::MouseButtonPressedEvent>,
	Ray::EventListener<Ray::MouseButtonReleasedEvent>
{
public:
	App()
	{
		Ray::Scene::Create<Scene>();

		// Create UI
		// Ray::Button& texturedButton = Ray::Scene::Get().CreateEntity().Emplace<Ray::Button>(
		//	"TexturedButton", Ray::Float2{ 100.0f, 10.0f }, 200.0f, 50.0f,
		//	Ray::Texture2D::Acquire("Resources/Textures/gridbase.png"));

		// Ray::Button& coloredButton =
		//	Ray::Scene::Get().CreateEntity().Emplace<Ray::Button>("ColoredButton",
		//		Ray::Float2{ 100.0f, 200.0f }, 200.0f, 50.0f, Ray::Float3{ 0.0f, 1.0f, 0.0f });

#include "../ImGuiWindows.inl"

		// Ray::ImGUI::Get().RegisterNewFrameFunction(
		//	[&]()
		//	{
		//		ImGui::Begin("Button settings");
		//		ImGui::ColorPicker3("Color", (float*)&m_ButtonColors);
		//		ImGui::End();
		//	});

		// Ray::ImGUI::Get().RegisterNewFrameFunction(
		//	[this]()
		//	{
		//		ImGui::Begin("Gridbase");
		//		ImGui::Image(Ray::ImGUI::Get().PushTexture(
		//						 *Ray::Texture2D::Acquire("Resources/Textures/gridbase.png")),
		//			ImVec2{ 512.0f, 512.0f });
		//		ImGui::End();
		//	});

		// Ray::ImGUI::Get().RegisterNewFrameFunction(
		//	[this]()
		//	{
		//		ImGui::Begin("FontTransform");

		//		Ray::Transform& tform = m_TextEntity.Get<Ray::Transform>();
		//		tform.SetTranslation(Ray::ImGUI::Float3Widget("Transform", tform.Translation()));
		//		tform.SetRotation(Ray::ImGUI::Float3Widget("Rotation", tform.Rotation()));
		//		tform.SetScale(Ray::ImGUI::Float3Widget("Scale", tform.Scale()));

		//		ImGui::End();
		//	});

		// auto font = Ray::Font::AcquireTTF("Resources/Fonts/Courier-Prime/Courier Prime.ttf", 48);

		// auto pipeline =
		//	Ray::GraphicsPipeline::Builder()
		//		.SetShader(Ray::Shader::AcquireSourceFile(
		//			{ "Resources/Shaders/Flat_Text.vert", "Resources/Shaders/Flat_Text.frag" }))
		//		.SetCullMode(VK_CULL_MODE_NONE)
		//		.Acquire();

		// auto textMaterial = Ray::Material::Builder(pipeline)
		//						.Set("samplerTextureAtlas", font->GetSharedTextureAtlas())
		//						.Acquire();

		// m_TextEntity = Scene::Get().CreateEntity();
		// m_TextEntity.Emplace<Ray::Mesh>(Ray::Mesh::Plane(textMaterial));
		// m_TextEntity.Emplace<Ray::MeshRenderer>(textMaterial);
		// m_TextEntity.Get<Ray::Transform>().Rotate({ Ray::Math::PI<> / 2.f, 0.f, 0.f });
		// m_TextEntity.Emplace<Ray::TextRenderer>(textMaterial, *font, 'A');

		// auto flatTextMaterial = Ray::MakeRef<Ray::FlatTextMaterial>(
		//	Ray::FlatTextMaterial::Layout{ "Hello World", font, { 1.0f, 1.0f, 1.0f, 1.0f } });

		// m_TextEntity = Scene::Get().CreateEntity();
		// m_TextEntity.Emplace<Ray::TextRenderer>(flatTextMaterial);

		// Scene::Get().CreateEntity().Emplace<Ray::Skybox>(
		//	Ray::Texture2D::Acquire("Resources/Textures/EquirectangularWorldMap.jpg"));

		auto shaderRenderGlyph = Ray::Shader::AcquireSourceFile(
			{ "Resources/Shaders/Text/Shared.vert", "Resources/Shaders/Text/RenderGlyph.frag" });

		auto shaderBlitAtlas = Ray::Shader::AcquireSourceFile(
			{ "Resources/Shaders/Text/Shared.vert", "Resources/Shaders/Text/BlitAtlas.frag" });

		auto shaderDrawText = Ray::Shader::AcquireSourceFile(
			{ "Resources/Shaders/Text/DrawText.vert", "Resources/Shaders/Text/DrawText.frag" });
	}

private:
	void OnEvent(Ray::HoverEnterEvent& e) override
	{
		Ray::Log::Warn("HoverEnterEvent {0}", e.GetWidget() ? e.GetWidget()->GetName() : "{Null}");
	}

	void OnEvent(Ray::HoverLeaveEvent& e) override
	{
		Ray::Log::Warn("HoverLeaveEvent {0}", e.GetWidget() ? e.GetWidget()->GetName() : "{Null}");
	}

	void OnEvent(Ray::MouseButtonPressedEvent& e) override
	{
		static bool gridbase = true;
		if (e.GetWidget() && e.GetWidget()->GetName() == "TexturedButton")
		{
			Ray::Button& btn = *(Ray::Button*)e.GetWidget();
			if (gridbase)
			{
				btn.SetTexture(
					Ray::Texture2D::Acquire("Resources/Textures/EquirectangularWorldMap.jpg"));
				gridbase = false;
			}
			else
			{
				btn.SetTexture(Ray::Texture2D::Acquire("Resources/Textures/gridbase.png"));
				gridbase = true;
			}
		}

		Ray::Log::Warn(
			"MouseButtonPressedEvent {0}", e.GetWidget() ? e.GetWidget()->GetName() : "{Null}");
	}

	void OnEvent(Ray::MouseButtonReleasedEvent& e) override
	{
		Ray::Log::Warn(
			"MouseButtonReleasedEvent {0}", e.GetWidget() ? e.GetWidget()->GetName() : "{Null}");
	}

	void Update() override
	{
		Ray::Scene::Get().EntityView<Ray::Button>().each(
			[this](Ray::Button& btn)
			{
				if (btn.GetName() == "ColoredButton")
					btn.SetColor(m_ButtonColors);
			});
	}

private:
	Ray::Float3 m_ButtonColors{};
	Ray::Entity m_TextEntity;
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
		Ray::Window::Create();
		Ray::Window::Get().Show();
		Ray::Window::Get().SetTitle("UITest");

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
