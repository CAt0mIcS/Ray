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

				// Ray::Transform& tform = m_Entity.Get<Ray::Transform>();

				// Ray::Float3& translation = const_cast<Ray::Float3&>(tform.Translation());
				// Ray::Float3& rotation = const_cast<Ray::Float3&>(tform.Rotation());
				// Ray::Float3& scale = const_cast<Ray::Float3&>(tform.Scale());

				// Ray::ImGUI::Float3Widget("Translation", translation);
				// Ray::ImGUI::Float3Widget("Rotation", rotation);
				// Ray::ImGUI::Float3Widget("Scale", scale);
				// ImGui::Spacing();

				// tform.RecalculateCachedMatrix();

				ImGui::End();
			}
		});

		using namespace At0::Ray;
		auto texture = MakeRef<Texture2D>("Resources/Textures/gridbase.png");

		{
			VertexShaderGenerator generator;

			auto cameraNode = MakeRef<CameraNode>();
			auto transformNode = MakeRef<TransformNode>();

			// generate vec4 from vec3
			auto vertexNode = MakeRef<VertexNode>();
			auto vec4Node = MakeRef<Vector4Node>();
			auto splitNode = MakeRef<SplitNode>();
			auto floatNode = MakeRef<FloatNode>(1.0f);

			splitNode.Connect(vertexNode, VertexNode::ScreenSpacePosition, SplitNode::Input);
			vec4Node.Connect(splitNode, SplitNode::R, Vector4Node::R);
			vec4Node.Connect(splitNode, SplitNode::G, Vector4Node::G);
			vec4Node.Connect(splitNode, SplitNode::B, Vector4Node::B);
			vec4Node.Connect(floatNode, FloatNode::Result, Vector4Node::A);


			// uScene.Proj * uScene.View
			auto multiplyNodeCamViewProj = MakeRef<MultiplyNode>();
			multiplyNodeCamViewProj.Connect(cameraNode, CameraNode::Projection, MultiplyNode::Left);
			multiplyNodeCamViewProj.Connect(cameraNode, CameraNode::View, MultiplyNode::Right);

			// (uScene.Proj * uScene.View) * uObj.Model
			auto multiplyNodeCamTrans = MakeRef<MultiplyNode>();
			multiplyNodeCamTrans.Connect(
				multiplyNodeCamViewProj, MultiplyNode::Result, MultiplyNode::Left);
			multiplyNodeCamTrans.Connect(transformNode, TransformNode::Model, MultiplyNode::Right);

			// ((uScene.Proj * uSceen.View) * uObj.Model) * inPos
			auto multiplyVertex = MakeRef<MultiplyNode>();
			multiplyVertex.Connect(multiplyNodeCamTrans, MultiplyNode::Result, MultiplyNode::Left);
			multiplyVertex.Connect(vec4Node, Vector4Node::Result, MultiplyNode::Right);
		}


		Ray::Scene::Get().CreateEntity().Emplace<Ray::Skybox>(
			Ray::MakeRef<Ray::Texture2D>("Resources/Textures/EquirectangularWorldMap.jpg"));
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
