#define RAY_ENABLE_IMGUI 0

#include <Ray/Core/REngine.h>
#include <Ray/Utils/RLogger.h>
#include <Ray/Devices/RWindow.h>
#include <Ray/Graphics/RGraphics.h>

#include <Ray/Components/RMesh.h>
#include <Ray/Components/RSkybox.h>
#include <Ray/Components/RTransform.h>

#include <Ray/Graphics/Images/RTexture.h>
#include <Ray/Graphics/Pipelines/RGraphicsPipeline.h>
#include <Ray/Utils/RException.h>
#include <Ray/Graphics/Pipelines/RGraphicsPipeline.h>
#include <Ray/Shading/RMaterial.h>
#include <Ray/Graphics/Pipelines/Shader/RShader.h>
#include <Ray/Components/RMeshRenderer.h>
#include <Ray/Scene/RScene.h>
#include <Ray/Layers/RLayer.h>
#include <Ray/Components/RModel.h>

#include <Ray/Scene/RCamera.h>

#include <signal.h>
#include <random>

#include <Ray/Core/RDynamicVertex.h>
#include <Ray/Graphics/Buffers/RVertexBuffer.h>
#include <Ray/Graphics/Buffers/RIndexBuffer.h>
#include <Ray/Events/REventListener.h>
#include <Ray/Events/REngineEvents.h>
#include <Ray/Events/RKeyboardEvents.h>
#include <Ray/Events/RMouseEvents.h>

#include <Ray/Components/UI/RTextComponent.h>

#include <Ray/Utils/RImGui.h>
#include <Ray/Graphics/Text/RFont.h>
#include <Ray/Components/UI/RButton.h>

#include <../../Extern/imgui/imgui.h>


using namespace At0::Ray;

class UiLayer :
	public Layer,
	EventListener<HoverEnterEvent>,
	EventListener<HoverLeaveEvent>,
	EventListener<MouseButtonPressedEvent>,
	EventListener<MouseButtonReleasedEvent>,
	EventListener<KeyPressedEvent>
{
public:
	UiLayer(Scene& scene)
		: Layer(scene), EventListener<HoverEnterEvent>(GetWindow()),
		  EventListener<HoverLeaveEvent>(GetWindow()),
		  EventListener<MouseButtonPressedEvent>(GetWindow()),
		  EventListener<MouseButtonReleasedEvent>(GetWindow()),
		  EventListener<KeyPressedEvent>(GetWindow())
	{
#include "../ImGuiWindows.inl"

		auto font = Font::AcquireTTF("Resources/Fonts/arial.ttf", 128);

		m_TextEntity = GetScene().CreateEntity();
		m_TextEntity.Emplace<TextComponent>(GetWindow(),
			"\"The beige hue on the waters of the loch impressed all, including the French queen, "
			"before she heard that symphony again, just as young Arthur wanted.\" (pangram)",
			font);


		// RAY_TODO: No longer working!
		m_ColoredBtn = GetScene().CreateEntity();
		m_ColoredBtn.Emplace<Button>(
			GetWindow(), "ColoredButton", Float2{ 0.f, 0.f }, 100.f, 100.f);


		// auto pipeline = PipelineBuilder()
		//					.SetShader(LoadShaderFromSourceFile(
		//						{ "Resources/Shaders/UI.vert", "Resources/Shaders/UI.frag" }))
		//					.SetCullMode(VK_CULL_MODE_NONE)
		//					.Build();

		// auto material =
		//	MaterialBuilder(pipeline).Set("Shading.color", Float4(1.f, 1.f, 1.f, 1.f)).Build();

		// GetScene().CreateEntity().Emplace<Mesh>(Mesh::Plane(material));

		// auto pipeline = GraphicsPipeline::Builder()
		//					.SetShader(Shader::FromSourceFile(
		//						{ "Resources/Shaders/UI.vert", "Resources/Shaders/UI.frag" }))
		//					.SetCullMode(VK_CULL_MODE_NONE)
		//					.Acquire();


		// auto material = Material::Builder(std::move(pipeline))
		//					.Set("Shading.color", Float4(1.f, 1.f, 1.f, 1.f))
		//					.Build();

		// GetScene().CreateEntity().Emplace<Mesh>(Mesh::Plane(std::move(material)));
	}


	void OnEvent(KeyPressedEvent& e) override
	{
		// auto& textComp = m_TextEntity.Get<TextComponent>();

		// if ((int)e.GetKey() >= 32 && (int)e.GetKey() <= 126 && e.GetKey() != Key::W &&
		//	e.GetKey() != Key::A && e.GetKey() != Key::S && e.GetKey() != Key::D)
		//{
		//	if (e.GetKey() == Key::Backspace)
		//		textComp.SetText(
		//			std::string(textComp.GetText().substr(0, textComp.GetText().size() - 1)));
		//	else
		//		textComp.SetText(std::string(textComp.GetText()) + String::Construct(e.GetKey()));
		// }
	}

	void OnEvent(HoverEnterEvent& e) override
	{
		Log::Warn("HoverEnterEvent {0}", e.GetWidget() ? e.GetWidget()->GetName() : "{Null}");
	}

	void OnEvent(HoverLeaveEvent& e) override
	{
		Log::Warn("HoverLeaveEvent {0}", e.GetWidget() ? e.GetWidget()->GetName() : "{Null}");
	}

	void OnEvent(MouseButtonPressedEvent& e) override
	{
		static bool gridbase = true;
		if (e.GetWidget() && e.GetWidget()->GetName() == "TexturedButton")
		{
			Button& btn = *(Button*)e.GetWidget();
			if (gridbase)
			{
				btn.SetTexture(Texture::Acquire("Resources/Textures/EquirectangularWorldMap.jpg"));
				gridbase = false;
			}
			else
			{
				btn.SetTexture(Texture::Acquire("Resources/Textures/gridbase.png"));
				gridbase = true;
			}
		}

		Log::Warn(
			"MouseButtonPressedEvent {0}", e.GetWidget() ? e.GetWidget()->GetName() : "{Null}");
	}

	void OnEvent(MouseButtonReleasedEvent& e) override
	{
		Log::Warn(
			"MouseButtonReleasedEvent {0}", e.GetWidget() ? e.GetWidget()->GetName() : "{Null}");
	}

private:
	virtual void Update(Delta dt) override
	{
		GetScene().EntityView<Button>().each(
			[this](Button& btn)
			{
				if (btn.GetName() == "ColoredButton")
					btn.SetColor(m_ButtonColors);
			});
	}

private:
	Float4 m_ButtonColors{};
	Entity m_TextEntity;

	Entity m_ColoredBtn;
	Entity m_TexturedBtn;
};


class NanosuitLayer : public Layer
{
public:
	NanosuitLayer(Scene& scene) : Layer(scene)
	{
		// auto pipeline =
		//	PipelineBuilder()
		//		.SetShader(LoadShaderFromSourceFile({ "Tests/Lighting/Shaders/Lighting.vert",
		//			"Tests/Lighting/Shaders/Lighting.frag" }))
		//		.SetCullMode(VK_CULL_MODE_NONE)
		//		.Acquire();

		// auto material = MaterialBuilder(std::move(pipeline))
		//					.Set("Shading.color", Float4{ 1.f })
		//					.Set("Shading.ambientLightColor", Float4{ 1.f, 1.f, 1.f, .1f })
		//					.Build();

		// GetScene().CreateEntity().Emplace<Model>(
		//	"Resources/Models/Nanosuit/nanosuit.obj", std::move(material));
	}
};


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

		RegisterLayer<UiLayer>();
		RegisterLayer<NanosuitLayer>();
	}
};


class App : public Engine
{
public:
	App() { Scene::Create<Scene2>(); };
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
	Log::SetLogLevel(At0::Violent::LogLevel::Debug);

	try
	{
		Window::Create();
		Window::Get().Show();
		Window::Get().SetTitle("UITest");

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
