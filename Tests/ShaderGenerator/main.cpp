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


#include <ShaderGraph/RShaderGenerator.h>

#include <ShaderGraph/Nodes/RCameraNode.h>
#include <ShaderGraph/Nodes/RFloatNode.h>
#include <ShaderGraph/Nodes/RMultiplyNode.h>
#include <ShaderGraph/Nodes/RSplitNode.h>
#include <ShaderGraph/Nodes/RTransformationMatrixNode.h>
#include <ShaderGraph/Nodes/RVector4Node.h>
#include <ShaderGraph/Nodes/RInputNode.h>
#include <ShaderGraph/Nodes/ROutputNode.h>
#include <ShaderGraph/Nodes/RVertexNode.h>
#include <ShaderGraph/Nodes/RVertexOutputNode.h>
#include <ShaderGraph/Nodes/RSampler2DNode.h>
#include <ShaderGraph/Nodes/RTexture2DNode.h>


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

static void WriteToFile(std::string_view str, const std::string& filename)
{
	std::filesystem::create_directory("Resources/Shaders/Generated/");
	std::ofstream writer("Resources/Shaders/Generated/" + filename);

	writer << str;
	writer.close();
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
		Ray::ImGUI::Get().RegisterNewFrameFunction([&]() {
			{
				static bool show = true;
				ImGui::ShowDemoWindow(&show);
			}
		});

		m_Texture = Ray::MakeRef<Ray::Texture2D>("Resources/Textures/gridbase.png");

		// Vertex Shader
		Ray::ShaderGenerator generator;
		auto rootNodes = GenerateVertexShader(generator);
		auto fsRootNodes = GenerateFragmentShader(generator);
		rootNodes.insert(rootNodes.begin(), fsRootNodes.begin(), fsRootNodes.end());
		fsRootNodes.clear();

		Ray::FlatColorMaterial::Layout layout{};

		Ray::GraphicsPipeline::Layout pipelineLayout{};
		pipelineLayout.cullMode = VK_CULL_MODE_NONE;
		pipelineLayout.shaders = { "Resources/Shaders/Generated/VertexShader.vert",
			"Resources/Shaders/Generated/FragmentShader.frag" };

		Ray::Ref<Ray::Material> material =
			Ray::MakeRef<Ray::FlatColorMaterial>(layout, pipelineLayout);

		m_Entity = Ray::Scene::Get().CreateEntity();
		m_Entity.Emplace<Ray::Mesh>(Ray::Mesh::Plane(material));
		auto& renderer = m_Entity.Emplace<Ray::MeshRenderer>(material, false);
		generator.AddUniforms(renderer);

		Ray::Scene::Get().CreateEntity().Emplace<Ray::Skybox>(
			Ray::MakeRef<Ray::Texture2D>("Resources/Textures/EquirectangularWorldMap.jpg"));

		m_Texture.reset();
	}

private:
	void Update() override {}

	std::vector<Ray::Ref<Ray::Node>> GenerateVertexShader(Ray::ShaderGenerator& generator)
	{
		using namespace At0::Ray;

		Time tStart = Time::Now();

		auto cameraNode = MakeRef<CameraNode>();
		auto transformNode = MakeRef<TransformationMatrixNode>();

		// generate vec4 from vec3
		auto vertexNode = MakeRef<VertexNode>();
		auto uvOutput = MakeRef<OutputNode>("vec2", "outUV");
		uvOutput->Connect(vertexNode, VertexNode::UV, OutputNode::Input);

		auto vec4Node = MakeRef<Vector4Node>();
		auto splitNode = MakeRef<SplitNode>();
		auto floatNode = MakeRef<FloatNode>(1.0f);

		splitNode->Connect(vertexNode, VertexNode::Position, SplitNode::Input);

		vec4Node->Connect(splitNode, SplitNode::R, Vector4Node::R);
		vec4Node->Connect(splitNode, SplitNode::G, Vector4Node::G);
		vec4Node->Connect(splitNode, SplitNode::B, Vector4Node::B);
		vec4Node->Connect(floatNode, FloatNode::Result, Vector4Node::A);

		// uScene.Proj * uScene.View
		auto multiplyNodeCamViewProj = MakeRef<MultiplyNode>();
		multiplyNodeCamViewProj->Connect(cameraNode, CameraNode::Projection, MultiplyNode::Left);
		multiplyNodeCamViewProj->Connect(cameraNode, CameraNode::View, MultiplyNode::Right);

		// (uScene.Proj * uScene.View) * uObj.Model
		auto multiplyNodeCamTrans = MakeRef<MultiplyNode>();
		multiplyNodeCamTrans->Connect(
			multiplyNodeCamViewProj, MultiplyNode::Result, MultiplyNode::Left);
		multiplyNodeCamTrans->Connect(
			transformNode, TransformationMatrixNode::Model, MultiplyNode::Right);

		// ((uScene.Proj * uSceen.View) * uObj.Model) * inPos
		auto multiplyVertex = MakeRef<MultiplyNode>();
		multiplyVertex->Connect(multiplyNodeCamTrans, MultiplyNode::Result, MultiplyNode::Left);
		multiplyVertex->Connect(vec4Node, Vector4Node::Result, MultiplyNode::Right);

		auto vertexOutputNode = MakeRef<VertexOutputNode>();
		vertexOutputNode->Connect(multiplyVertex, MultiplyNode::Result, VertexOutputNode::Vertex);

		WriteToFile(
			generator.GenerateVertexShader({ vertexOutputNode, uvOutput }), "VertexShader.vert");

		Log::Info("Vertex shader generation took {0}ms", (Time::Now() - tStart).AsMilliseconds());
		return { vertexOutputNode, uvOutput };
	}


	std::vector<Ray::Ref<Ray::Node>> GenerateFragmentShader(Ray::ShaderGenerator& generator)
	{
		using namespace At0::Ray;

		Time tStart = Time::Now();

		auto uvInput = MakeRef<InputNode>("vec2", "inUV");
		auto textureNode = MakeRef<Texture2DNode>(m_Texture);
		auto samplerNode = MakeRef<Sampler2DNode>();

		samplerNode->Connect(textureNode, Texture2DNode::Output, Sampler2DNode::Texture);
		samplerNode->Connect(uvInput, InputNode::Result, Sampler2DNode::UV);

		auto outColorNode = MakeRef<OutputNode>("vec4", "outColor");
		outColorNode->Connect(samplerNode, Sampler2DNode::Result, OutputNode::Input);

		WriteToFile(generator.GenerateFragmentShader({ outColorNode }), "FragmentShader.frag");

		Log::Info("Fragment shader generation took {0}ms", (Time::Now() - tStart).AsMilliseconds());

		return { outColorNode };
	}

private:
	Ray::Entity m_Entity;
	Ray::Ref<Ray::Texture2D> m_Texture;
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
