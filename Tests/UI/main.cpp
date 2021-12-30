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

#include <Ray/Events/REventListener.h>
#include <Ray/Events/REngineEvents.h>
#include <Ray/Events/RKeyboardEvents.h>
#include <Ray/Events/RMouseEvents.h>

#include <Ray/UI/RImGui.h>
#include <Ray/Graphics/Text/RFont.h>
#include <Ray/UI/RButton.h>

#include <../../Extern/imgui/imgui.h>

#include "../../Extern/VEFontCache/ve_fontcache.h"


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

		auto shaderRenderGlyph = Shader::AcquireSourceFile(
			{ "Resources/Shaders/Text/Shared.vert", "Resources/Shaders/Text/RenderGlyph.frag" });

		auto shaderBlitAtlas = Shader::AcquireSourceFile(
			{ "Resources/Shaders/Text/Shared.vert", "Resources/Shaders/Text/BlitAtlas.frag" });

		auto shaderDrawText = Shader::AcquireSourceFile(
			{ "Resources/Shaders/Text/DrawText.vert", "Resources/Shaders/Text/DrawText.frag" });

		auto sampler = TextureSampler::Builder()
						   .SetMinFilter(VK_FILTER_NEAREST)
						   .SetMagFilter(VK_FILTER_NEAREST)
						   .SetAddressModeU(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE)
						   .SetAddressModeV(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE)
						   .SetAddressModeW(VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE)
						   .BuildScoped();

		auto firstTexture =
			Texture::Builder()
				.SetExtent(
					{ VE_FONTCACHE_GLYPHDRAW_BUFFER_WIDTH, VE_FONTCACHE_GLYPHDRAW_BUFFER_HEIGHT })
				.SetFormat(VK_FORMAT_R8_UNORM)
				.SetImageUsage(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT)
				.SetMemoryProperties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
				.SetTextureSampler(std::move(sampler))
				.Build();
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
