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
#include <Ray/Components/RHierachyComponent.h>

#include <Ray/Graphics/Images/RTexture2D.h>
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

#include <Ray/Mono/RScript.h>


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

		Ray::ImGUI::Get().RegisterNewFrameFunction(
			[&]()
			{
				{
					auto translate = [](Ray::Transform& tform)
					{
						Ray::Float3 newTranslation =
							Ray::ImGUI::Float3Widget("Translation", tform.Translation());
						Ray::Float3 newRotation =
							Ray::ImGUI::Float3Widget("Rotation", tform.Rotation());
						Ray::Float3 newScale = Ray::ImGUI::Float3Widget("Scale", tform.Scale());

						if (newTranslation != tform.Translation())
							tform.SetTranslation(newTranslation);
						if (newRotation != tform.Rotation())
							tform.SetRotation(newRotation);
						if (newScale != tform.Scale())
							tform.SetScale(newScale);
					};

					{
						if (m_Parent)
						{
							ImGui::Begin("Parent");
							translate(m_Parent.Get<Ray::Transform>());
							ImGui::Spacing();
							ImGui::End();
						}
					}
					{
						if (m_Child)
						{
							ImGui::Begin("Child");
							translate(m_Child.Get<Ray::Transform>());
							ImGui::Spacing();
							ImGui::End();
						}
					}
					{
						ImGui::Begin("Testing");
						Ray::Float3 oldColor = m_Material->Get<Ray::Float3>("Shading.color");
						Ray::Float3 newColor = Ray::ImGUI::Float3Widget("Color", oldColor);
						if (oldColor != newColor)
							m_Material->Set("Shading.color", Ray::Float4{ newColor, 1.0f });

						if (ImGui::Button("Create/Delete Child"))
						{
							if (m_Child == Ray::Entity::Null)
							{
								CreateChild();
							}
							else
							{
								Scene::Get().DestroyEntity(m_Child);
								m_Child = Ray::Entity::Null;
							}
						}
						if (ImGui::Button("Create/Delete Parent"))
						{
							if (m_Parent == Ray::Entity::Null)
							{
								CreateParent();
							}
							else
							{
								Scene::Get().DestroyEntity(m_Parent);
								m_Parent = Ray::Entity::Null;
								m_Child = Ray::Entity::Null;
							}
						}
						ImGui::End();
					}
				}
			});
#include "../ImGuiWindows.inl"

		auto shader =
			Ray::Shader::Acquire({ "Resources/Shaders/Test.vert", "Resources/Shaders/Test.frag" });

		auto pipeline = Ray::GraphicsPipeline::Builder()
							.SetShader(shader)
							.SetCullMode(VK_CULL_MODE_NONE)
							.Acquire();
		m_Material =
			Ray::Material::Builder(pipeline)
				.Set("samplerDiffuse", Ray::Texture2D::Acquire("Resources/Textures/gridbase.png"))
				.Set("Shading.color", Ray::Float4{ 1.0f, 0.4f, 0.134f, 1.0f })
				.Acquire();

		m_Parent = Scene::Get().CreateEntity();
		m_Parent.Emplace<Ray::Mesh>(Ray::Mesh::Plane(m_Material));
		Ray::MeshRenderer& rendererParent = m_Parent.Emplace<Ray::MeshRenderer>(m_Material);

		m_Material->Set("samplerDiffuse",
			Ray::Texture2D::Acquire("Resources/Textures/EquirectangularWorldMap.jpg"));

		// RAY_TODO: Move constructors for MeshRenderer are missing, so when enTT moves (resize due
		// to new MeshRenderer) the listener is still active on an invalid renderer

		m_Child = Scene::Get().CreateEntity();
		m_Child.Get<Ray::Transform>().Translate({ 4.0f, 0.0f, 0.0f });
		m_Child.Emplace<Ray::Mesh>(Ray::Mesh::Plane(m_Material));
		Ray::MeshRenderer& rendererChild = m_Child.Emplace<Ray::MeshRenderer>(m_Material);

		m_Parent.Emplace<Ray::HierachyComponent>().AddChild(m_Child);
		m_Child.Emplace<Ray::HierachyComponent>().SetParent(m_Parent);
	}

private:
	void Update() override {}

	void CreateParent()
	{
		m_Parent = Scene::Get().CreateEntity();
		m_Parent.Get<Ray::Transform>().Translate({ 4.0f, 0.0f, 0.0f });
		m_Parent.Emplace<Ray::Mesh>(Ray::Mesh::Plane(m_Material));
		Ray::MeshRenderer& rendererParent = m_Parent.Emplace<Ray::MeshRenderer>(m_Material);

		CreateChild();
		m_Parent.Emplace<Ray::HierachyComponent>().AddChild(m_Child);
	}

	void CreateChild()
	{
		m_Child = Scene::Get().CreateEntity();
		m_Child.Get<Ray::Transform>().Translate({ 4.0f, 0.0f, 0.0f });
		m_Child.Emplace<Ray::Mesh>(Ray::Mesh::Plane(m_Material));
		Ray::MeshRenderer& rendererChild = m_Child.Emplace<Ray::MeshRenderer>(m_Material);
		m_Child.Emplace<Ray::HierachyComponent>().SetParent(m_Parent);
	}

private:
	Ray::Entity m_Child;
	Ray::Entity m_Parent;
	Ray::Ref<Ray::Material> m_Material;
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
	Ray::Log::SetLogLevel(Violent::LogLevel::Trace);

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
