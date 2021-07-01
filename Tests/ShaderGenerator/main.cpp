#include <signal.h>

#include <Ray/Scene/RScene.h>
#include <Ray/Scene/REntity.h>
#include <Ray/Scene/RCamera.h>
#include <Ray/Core/REngine.h>
#include <Ray/Devices/RWindow.h>
#include <Ray/Utils/RException.h>
#include <Ray/Utils/RLogger.h>
#include <Ray/Graphics/Images/RTexture2D.h>

#include <Ray/Shading/Flat/RFlatColorMaterial.h>

#include <Ray/Core/RRendererLoader.h>
#include <Ray/Components/RMesh.h>
#include <Ray/Components/RMeshRenderer.h>
#include <Ray/Components/RTransform.h>
#include <Ray/Components/RSkybox.h>

#include <Ray/UI/RImGui.h>
#include <../../Extern/imgui/imgui.h>


#include <Ray/ShaderGraph/RShaderGenerator.h>

#include <Ray/ShaderGraph/Nodes/RCameraNode.h>
#include <Ray/ShaderGraph/Nodes/RFloatNode.h>
#include <Ray/ShaderGraph/Nodes/RMultiplyNode.h>
#include <Ray/ShaderGraph/Nodes/RSplitNode.h>
#include <Ray/ShaderGraph/Nodes/RTransformationMatrixNode.h>
#include <Ray/ShaderGraph/Nodes/RVector4Node.h>
#include <Ray/ShaderGraph/Nodes/RInputNode.h>
#include <Ray/ShaderGraph/Nodes/ROutputNode.h>
#include <Ray/ShaderGraph/Nodes/RVertexNode.h>
#include <Ray/ShaderGraph/Nodes/RVertexOutputNode.h>
#include <Ray/ShaderGraph/Nodes/RSampler2DNode.h>
#include <Ray/ShaderGraph/Nodes/RTexture2DNode.h>
#include <Ray/ShaderGraph/Nodes/RPropertyNode.h>


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
				ImGui::Begin("ColorPicker");

				float col[4] = { m_Color.x, m_Color.y, m_Color.z, m_Color.w };
				ImGui::ColorPicker4("TextureColor", col);
				m_Color.x = col[0];
				m_Color.y = col[1];
				m_Color.z = col[2];
				m_Color.w = col[3];

				ImGui::End();
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
		pipelineLayout.shader =
			Ray::Shader::FromGlsl({ "Resources/Shaders/Generated/VertexShader.vert",
				"Resources/Shaders/Generated/FragmentShader.frag" });

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
	void Update() override
	{
		m_Entity.Get<Ray::MeshRenderer>().GetBufferUniform("Shading")["color"] = m_Color;
	}

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
		auto colorInputNode = MakeRef<PropertyNode>("Shading", "vec4", "color");

		samplerNode->Connect(textureNode, Texture2DNode::Output, Sampler2DNode::Texture);
		samplerNode->Connect(uvInput, InputNode::Result, Sampler2DNode::UV);

		auto multiplyNode = MakeRef<MultiplyNode>();
		multiplyNode->Connect(samplerNode, Sampler2DNode::Result, MultiplyNode::Left);
		multiplyNode->Connect(colorInputNode, PropertyNode::Result, MultiplyNode::Right);

		auto outColorNode = MakeRef<OutputNode>("vec4", "outColor");
		outColorNode->Connect(multiplyNode, MultiplyNode::Result, OutputNode::Input);

		WriteToFile(generator.GenerateFragmentShader({ outColorNode }), "FragmentShader.frag");

		Log::Info("Fragment shader generation took {0}ms", (Time::Now() - tStart).AsMilliseconds());

		return { outColorNode };
	}

private:
	Ray::Entity m_Entity;
	Ray::Ref<Ray::Texture2D> m_Texture;
	Ray::Float4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };
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
		Ray::LoadRenderer(Ray::RendererAPI::Vulkan);

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
