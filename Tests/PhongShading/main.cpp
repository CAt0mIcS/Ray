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

		ImGUI::Get().RegisterNewFrameFunction(
			[&]()
			{
				{
					ImGui::Begin("GlobalLight");

					Float4 ambientLightColor = m_Material->Get<Float4>("Shading.ambientLightColor");

					ambientLightColor = Float4{ ImGUI::Float3Widget("AmbientLightColor",
													Float3{ ambientLightColor }),
						ambientLightColor.w };

					ImGui::SliderFloat("Intensity", &ambientLightColor.w, 0.001f, .5f);
					m_Material->Set("Shading.ambientLightColor", ambientLightColor);

					ImGui::End();
				}

				for (int i = 0; i < m_Lights.size(); ++i)
				{
					ImGui::Begin(("Light_" + std::to_string(i)).c_str());

					std::string index = "[" + std::to_string(i) + "]";

					Float3 lightPos = ImGUI::Float3Widget(
						"LightPos", m_Material->Get<Float3>("Shading.lightPosition" + index));
					m_Material->Set("Shading.lightPosition" + index, Float4{ lightPos, 1.f });

					m_Lights[i].Get<Transform>().SetTranslation(lightPos);

					PointLight& pointLight = m_Lights[i].Get<PointLight>();
					pointLight.SetColor(
						{ ImGUI::Float3Widget("LightColor", pointLight.GetColor()), 1.f });

					ImGui::End();
				}
			});

		m_Entity = Scene::Get().CreateEntity();
		m_Entity.Emplace<Mesh>(Mesh::Import("Resources/Models/Plane.obj"));
		m_Entity.Get<Transform>().SetTranslation({ 0.f, .5f, 0.f }).SetScale(Float3{ 6.f });
		m_Material = m_Entity.Get<MeshRenderer>().GetSharedMaterial();

		Entity smoothVase = Scene::Get().CreateEntity();
		smoothVase.Emplace<Mesh>(Mesh::Import("Resources/Models/SmoothVase.obj", m_Material));
		smoothVase.Get<Transform>().SetTranslation(Float3{ 0.f, .4f, 0.f }).SetScale(Float3{ 3.f });

		Entity flatVase = Scene::Get().CreateEntity();
		flatVase.Emplace<Mesh>(Mesh::Import("Resources/Models/FlatVase.obj", m_Material));
		flatVase.Get<Transform>().SetTranslation(Float3{ 4.f, .4f, 0.f }).SetScale(Float3{ 3.f });

		auto flatColorPipeline =
			GraphicsPipeline::Builder()
				.SetShader(Shader::AcquireSourceFile(
					{ "Resources/Shaders/Flat_Col.vert", "Resources/Shaders/Flat_Col.frag" }))
				.Acquire();

		// for (int i = 0; i < m_Material->Get<uint32_t>("Shading.numLights"); ++i)
		//	m_Lights.emplace_back(Scene::Get().CreateEntity())
		//		.Emplace<Mesh>(Mesh::UVSphere(flatWhiteMaterial, .1f, 24, 24));

		// Entity light0 = m_Lights.emplace_back(Scene::Get().CreateEntity());
		// light0.Emplace<PointLight>();
		// light0.Emplace<Mesh>(Mesh::UVSphere(flatWhiteMaterial, .1f, 24, 24));

		// Entity light1 = m_Lights.emplace_back(Scene::Get().CreateEntity());
		// light1.Emplace<PointLight>(Float4{ 1.f, 0.f, 0.f, 1.f });
		// light1.Emplace<Mesh>(Mesh::UVSphere(flatWhiteMaterial, .1f, 24, 24));

		std::vector<Float4> lightColors{ { 1.f, .1f, .1f, 1.f }, { .1f, .1f, 1.f, 1.f },
			{ .1f, 1.f, .1f, 1.f }, { 1.f, 1.f, .1f, 1.f }, { .1f, 1.f, 1.f, 1.f },
			{ 1.f, 1.f, 1.f, 1.f } };

		for (int i = 0; i < lightColors.size(); ++i)
		{
			auto flatMaterial =
				Material::Builder(flatColorPipeline).Set("Shading.color", lightColors[i]).Acquire();

			Entity light = m_Lights.emplace_back(Scene::Get().CreateEntity());

			Matrix rotateLight = glm::rotate(Matrix{ 1.f },
				(i * glm::two_pi<float>()) / lightColors.size(), { 0.f, -1.5f, 0.f });

			light.Get<Transform>().SetTranslation(rotateLight * Float4{ -1.f, -1.f, -1.f, 1.f });

			PointLight& pointLight = light.Emplace<PointLight>(lightColors[i]);
			light.Emplace<Mesh>(Mesh::UVSphere(flatMaterial, .1f, 24, 24));
		}
	}

private:
	void Update() override
	{
		for (Entity light : m_Lights)
		{
			Matrix rotateLight =
				glm::rotate(Matrix{ 1.f }, GetDelta().AsSeconds(), { 0.f, -1.f, 0.f });

			light.Get<Transform>().SetTranslation(
				rotateLight * Float4{ light.Get<Transform>().Translation(), 1.f });
		}

		Scene::Get().EntityView<MeshRenderer>().each(
			[this](MeshRenderer& renderer)
			{
				for (Entity light : m_Lights)
				{
					std::string id = "[" + std::to_string(light.Get<PointLight>().GetID()) + "]";

					Material& material = renderer.GetMaterial();
					if (!material.HasUniform("Shading.lightPosition" + id))
						return;

					material.Set("Shading.lightPosition" + id,
						Float4{ light.Get<Transform>().Translation(), 1.f });
				}
			});
	}

private:
	Entity m_Entity;
	std::vector<Entity> m_Lights;
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
