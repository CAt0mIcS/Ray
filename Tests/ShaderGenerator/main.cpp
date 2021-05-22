#include <signal.h>

#include <Scene/RScene.h>
#include <Scene/REntity.h>
#include <Scene/RCamera.h>
#include <Core/REngine.h>
#include <Devices/RWindow.h>
#include <Utils/RException.h>
#include <Utils/RLogger.h>
#include <Graphics/Images/RTexture2D.h>

#include <Shading/Flat/RFlatColorMaterial.h>

#include <Components/RMesh.h>
#include <Components/RMeshRenderer.h>
#include <Components/RTransform.h>
#include <Components/RSkybox.h>

#include <UI/RImGui.h>
#include <../../Extern/imgui/imgui.h>


#include <ShaderGraph/RFlatShaderGenerator.h>
#include <ShaderGraph/Techniques/RFloat4Technique.h>
#include <ShaderGraph/Techniques/RMultiplyTechnique.h>
#include <ShaderGraph/Techniques/RTexture2DTechnique.h>
#include <ShaderGraph/Techniques/RSampler2DTechnique.h>

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

std::vector<std::string> WriteToFiles(std::vector<std::string> codes, std::string name = "Shader")
{
	std::filesystem::create_directories("Resources/Shaders/Generated/");
	std::string vertexShaderPath = "Resources/Shaders/Generated/" + name + ".vert";
	std::string fragmentShaderPath = "Resources/Shaders/Generated/" + name + ".frag";

	{
		std::ofstream writer(vertexShaderPath);
		writer << codes[0];
		writer.close();
	}
	{
		std::ofstream writer(fragmentShaderPath);
		writer << codes[1];
		writer.close();
	}

	return { vertexShaderPath, fragmentShaderPath };
}


class App : public Ray::Engine
{
public:
	App()
	{
		Ray::Scene::Create<Scene>();
		Ray::ImGUI::Get().RegisterNewFrameFunction([&]() {
			{
				ImGui::Begin("TestEntity");

				Ray::Transform& tform = m_Entity.Get<Ray::Transform>();

				Ray::Float3& translation = const_cast<Ray::Float3&>(tform.Translation());
				Ray::Float3& rotation = const_cast<Ray::Float3&>(tform.Rotation());
				Ray::Float3& scale = const_cast<Ray::Float3&>(tform.Scale());

				Ray::ImGUI::Float3Widget("Translation", translation);
				Ray::ImGUI::Float3Widget("Rotation", rotation);
				Ray::ImGUI::Float3Widget("Scale", scale);
				ImGui::Spacing();

				tform.RecalculateCachedMatrix();

				ImGui::End();
			}
		});


		auto texture = Ray::MakeRef<Ray::Texture2D>("Resources/Textures/gridbase.png");
		Ray::Float4 colorTechVal = { 1.0f, 1.0f, 0.0f, 1.0f };
		std::vector<std::string> shaderCodes;
		{
			Ray::Time tStart = Ray::Time::Now();

			auto texTech = Ray::MakeScope<Ray::Texture2DTechnique>(texture);

			auto samplerTech = Ray::MakeScope<Ray::Sampler2DTechnique>();
			samplerTech->Connect(Ray::Sampler2DTechnique::Texture, std::move(texTech));

			auto colorTech = Ray::MakeScope<Ray::Float4Technique>(colorTechVal, "Shading", "color");

			auto multiptlyTech = Ray::MakeScope<Ray::MultiplyTechnique>();
			multiptlyTech->Connect(Ray::MultiplyTechnique::Left, std::move(samplerTech));
			multiptlyTech->Connect(Ray::MultiplyTechnique::Right, std::move(colorTech));

			Ray::FlatShaderGenerator generator;
			generator.Connect(Ray::FlatShaderGenerator::Color, std::move(multiptlyTech));

			shaderCodes = generator.Generate();
			Ray::Log::Info(
				"Shader generator took {0}ms", (Ray::Time::Now() - tStart).AsMilliseconds());
		}


		std::vector<std::string> shaderPaths =
			WriteToFiles(std::move(shaderCodes), "FlatStaticColor");

		Ray::GraphicsPipeline::Layout pipelineLayout{};
		pipelineLayout.shaders = shaderPaths;
		pipelineLayout.cullMode = VK_CULL_MODE_NONE;

		auto colorMaterial = Ray::MakeRef<Ray::FlatColorMaterial>(
			Ray::FlatColorMaterial::Layout{}, std::move(pipelineLayout));

		m_Entity = Scene::Get().CreateEntity();
		m_Entity.Emplace<Ray::Mesh>(Ray::Mesh::Plane(colorMaterial));
		auto& meshRenderer = m_Entity.Emplace<Ray::MeshRenderer>(colorMaterial, false);
		meshRenderer.AddSampler2DUniform(
			"sampler2D_0", Ray::Shader::Stage::Fragment, std::move(texture));
		meshRenderer.AddBufferUniform("Shading", Ray::Shader::Stage::Fragment)["color"] =
			colorTechVal;

		Scene::Get().CreateEntity().Emplace<Ray::Skybox>(
			Ray::MakeRef<Ray::Texture2D>("Resources/Textures/EquirectangularWorldMap.jpg"));
	}

private:
	void Update() override
	{
		static Ray::Time tStart = Ray::Time::Now();

		Ray::Float4 sinColor(sin((Ray::Time::Now() - tStart).AsSeconds()),
			cos((Ray::Time::Now() - tStart).AsSeconds()),
			tan((Ray::Time::Now() - tStart).AsSeconds()), 1.0f);
		m_Entity.Get<Ray::MeshRenderer>().GetBufferUniform("Shading")["color"] = sinColor;
	}

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
