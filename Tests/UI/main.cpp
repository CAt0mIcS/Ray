#define RAY_ENABLE_IMGUI 0

#include <Ray/Core/REngine.h>
#include <Ray/Utils/RLogger.h>
#include <Ray/Devices/RWindow.h>
#include <Ray/Graphics/RGraphics.h>

#include <Ray/Components/RMesh.h>
#include <Ray/Components/RTextRenderer.h>
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

#include <Ray/UI/RImGui.h>
#include <Ray/Graphics/Text/RFont.h>
#include <Ray/UI/RButton.h>

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


class App :
	public Engine,
	EventListener<HoverEnterEvent>,
	EventListener<HoverLeaveEvent>,
	EventListener<MouseButtonPressedEvent>,
	EventListener<MouseButtonReleasedEvent>
{
public:
	App()
	{
		Scene::Create<Scene2>();
#include "../ImGuiWindows.inl"

		auto font = Font::AcquireTTF("Resources/Fonts/arial.ttf", 128);

		auto pipeline =
			GraphicsPipeline::Builder()
				.SetShader(Shader::AcquireSourceFile(
					{ "Resources/Shaders/Flat_Text.vert", "Resources/Shaders/Flat_Text.frag" }))
				.SetCullMode(VK_CULL_MODE_NONE)
				.Acquire();

		auto placeholderPipeline =
			GraphicsPipeline::Builder()
				.SetShader(Shader::AcquireSourceFile(
					{ "Resources/Shaders/Flat_Col.vert", "Resources/Shaders/Flat_Col.frag" }))
				.SetCullMode(VK_CULL_MODE_NONE)
				.Acquire();

		std::string text = "Hello World";

		float x{};
		float y{};
		float scale = 1.f;
		for (uint8_t c : text)
		{
			const Font::Glyph& glyph = font->GetGlyph(c);

			Ref<Material> textMaterial;

			if (glyph.texture)
				textMaterial =
					Material::Builder(pipeline).Set("samplerText", glyph.texture).Build();
			else
				textMaterial = Material::Builder(placeholderPipeline)
								   .Set("Shading.color", Float4{ 1.f, 0.f, 0.f, 1.f })
								   .Acquire();

			UInt2 windowSize = Window::Get().GetFramebufferSize();

			// Float2 ndcBearing{ (Float2)glyph.bearing / (Float2)windowSize };
			// Float2 ndcSize{ (Float2)glyph.size / (Float2)windowSize };

			Float2 ndcBearing = ScreenSpaceToNDCSpace(glyph.bearing) + Float2{ 1.f, 0.f };
			Float2 ndcSize = ScreenSpaceToNDCSpace(glyph.size) + Float2{ 1.f, 0.f };

			float xPos = x + ndcBearing.x * scale;
			float yPos = y - (ndcSize.y - ndcBearing.y) * scale;

			float w = ndcSize.x * scale;
			float h = ndcSize.y * scale;

			m_TextEntity = Scene::Get().CreateEntity();
			m_TextEntity.Emplace<Mesh>(Mesh::Plane(textMaterial));
			auto& tform = m_TextEntity.Get<Transform>();
			tform.SetRotation({ 3 * Math::PI<> / 2.f, Math::PI<>, 0.f });
			tform.SetScale({ w, 1.f, h });
			tform.SetTranslation({ xPos, yPos, 0.f });

			x += ScreenSpaceToNDCSpaceX(glyph.advance * scale) + 1.f;
			// x += glyph.advance * scale / windowSize.x;
		}
	}

private:
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

	void Update() override
	{
		Scene::Get().EntityView<Button>().each(
			[this](Button& btn)
			{
				if (btn.GetName() == "ColoredButton")
					btn.SetColor(m_ButtonColors);
			});
	}

private:
	Float3 m_ButtonColors{};
	Entity m_TextEntity;
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
