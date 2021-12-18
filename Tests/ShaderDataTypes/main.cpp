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
#include <Ray/Components/RTagComponent.h>

#include <Ray/Graphics/Images/RTexture2D.h>
#include <Ray/Graphics/Images/RTextureCubemap.h>
#include <Ray/Graphics/Pipelines/RGraphicsPipeline.h>
#include <Ray/Utils/RException.h>
#include <Ray/Graphics/Pipelines/Shader/RShader.h>
#include <Ray/Core/RDynamicVertex.h>
#include <Ray/Graphics/Buffers/RUniformBuffer.h>

#include <Ray/Scene/RScene.h>
#include <Ray/Scene/RCamera.h>
#include <Ray/Shading/RMaterial.h>

#include <signal.h>
#include <random>
#include <filesystem>

#include <Ray/UI/RImGui.h>
#include <../../Extern/imgui/imgui.h>

#include <Ray/Mono/RScript.h>


namespace At0::Raditor
{
	class Scene : public Ray::Scene
	{
	public:
		Scene() : Ray::Scene(Ray::MakeScope<Ray::Camera>())
		{
			Ray::UInt2 size = Ray::Window::Get().GetFramebufferSize();
			GetCamera().Type = Ray::Camera::LookAt;
			GetCamera().SetPosition(Ray::Float3(0.0f, 0.0f, -2.5f));
			GetCamera().SetRotation(Ray::Float3(0.0f));
			GetCamera().SetRotationSpeed(0.12f);
			GetCamera().SetPerspective(60.0f, (float)size.x / (float)size.y, 0.1f, 512.0f);
			GetCamera().SetMovementSpeed(3.0f);
		}
	};

	class App : public Ray::Engine, Ray::EventListener<Ray::CameraChangedEvent>
	{
	public:
		App()
		{
			std::string vertexShader = R"(
#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) in vec3 inPos;

layout(set = 0, binding = 0) uniform PerSceneData
{
	mat4 View;
	mat4 Proj;
} uScene;

layout(set = 1, binding = 1) uniform PerObjectData
{
	mat4 Model;
} uObj;

void main()
{
	gl_Position = uScene.Proj * uScene.View * uObj.Model * vec4(inPos, 1.0f);
}
)";

			std::string fragmentShader = R"(
#version 450 core
#extension GL_ARB_separate_shader_objects : enable


layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 2) uniform Shading
{
	float f;
	vec2 v2;
	vec3 v3;
	vec4 v4;
	int i;
	ivec2 iv2;
	ivec3 iv3;
	ivec4 iv4;
	uint ui;
	uvec2 uv2;
	uvec3 uv3;
	uvec4 uv4;
	bool b;
	bvec2 bv2;
	bvec3 bv3;
	bvec4 bv4;
	double d;
	dvec2 dv2;
	dvec3 dv3;
	dvec4 dv4;
	mat2 m2;
	mat2x3 m2x3;
	mat2x4 m2x4;
	mat3 m3;
	mat3x2 m3x2;
	mat3x4 m3x4;
	mat4 m4;
	mat4x2 m4x2;
	mat4x3 m4x3;
	dmat2 dm2;
	dmat2x3 dm2x3;
	dmat2x4 dm2x4;
	dmat3 dm3;
	dmat3x2 dm3x2;
	dmat3x4 dm3x4;
	dmat4 dm4;
	dmat4x2 dm4x2;
	dmat4x3 dm4x3;

	vec4 color;
} uShading;


void main()
{
	uShading.f;
	uShading.v2;
	uShading.v3;
	uShading.v4;
	uShading.i;
	uShading.iv2;
	uShading.iv3;
	uShading.iv4;
	uShading.ui;
	uShading.uv2;
	uShading.uv3;
	uShading.uv4;
	uShading.b;
	uShading.bv2;
	uShading.bv3;
	uShading.bv4;
	uShading.d;
	uShading.dv2;
	uShading.dv3;
	uShading.dv4;
	uShading.m2;
	uShading.m2x3;
	uShading.m2x4;
	uShading.m3;
	uShading.m3x2;
	uShading.m3x4;
	uShading.m4;
	uShading.m4x2;
	uShading.m4x3;
	uShading.dm2;
	uShading.dm2x3;
	uShading.dm2x4;
	uShading.dm3;
	uShading.dm3x2;
	uShading.dm3x4;
	uShading.dm4;
	uShading.dm4x2;
	uShading.dm4x3;

	outColor = uShading.color;
}

)";


			Ray::Scene::Create<Scene>();
			RegisterForDispatcher(Scene::Get().GetCamera());

			Scene::Get().CreateEntity().Emplace<Ray::Mesh>(
				Ray::Mesh::Import("Resources/Models/Nanosuit/nanosuit.obj"));

			auto pipeline =
				Ray::GraphicsPipeline::Builder()
					.SetShader(Ray::Shader::AcquireSourceString({ vertexShader, fragmentShader },
						{ Ray::ShaderStage::Vertex, Ray::ShaderStage::Fragment }))
					.SetCullMode(VK_CULL_MODE_NONE)
					.Acquire();

			auto material = Ray::Material::Builder(pipeline)
								.Set("Shading.color", Ray::Float4{ .7f, .2f, .2f, 1.f })
								.Set("Shading.f", 1.0f)
								.Set("Shading.v2", Ray::Float2{})
								.Set("Shading.v3", Ray::Float3{})
								.Set("Shading.v4", Ray::Float4{})
								.Set("Shading.i", 3)
								.Set("Shading.iv2", Ray::Int2{})
								.Set("Shading.iv3", Ray::Int3{})
								.Set("Shading.iv4", Ray::Int4{})
								.Set("Shading.ui", 3u)
								.Set("Shading.uv2", Ray::UInt2{})
								.Set("Shading.uv3", Ray::UInt3{})
								.Set("Shading.uv4", Ray::UInt4{})
								.Set("Shading.b", true)
								.Set("Shading.bv2", Ray::Bool2{})
								.Set("Shading.bv3", Ray::Bool3{})
								.Set("Shading.bv4", Ray::Bool4{})
								.Set("Shading.d", 3.0)
								.Set("Shading.dv2", Ray::Double2{})
								.Set("Shading.dv3", Ray::Double3{})
								.Set("Shading.dv4", Ray::Double4{})
								.Set("Shading.m2", Ray::Matrix2{})
								.Set("Shading.m2x3", Ray::Matrix2x3{})
								.Set("Shading.m2x4", Ray::Matrix2x4{})
								.Set("Shading.m3", Ray::Matrix3{})
								.Set("Shading.m3x2", Ray::Matrix3x2{})
								.Set("Shading.m3x4", Ray::Matrix3x4{})
								.Set("Shading.m4", Ray::Matrix4{})
								.Set("Shading.m4x2", Ray::Matrix4x2{})
								.Set("Shading.m4x3", Ray::Matrix4x3{})
								.Set("Shading.dm2", Ray::DMatrix2{})
								.Set("Shading.dm2x3", Ray::DMatrix2x3{})
								.Set("Shading.dm2x4", Ray::DMatrix2x4{})
								.Set("Shading.dm3", Ray::DMatrix3{})
								.Set("Shading.dm3x2", Ray::DMatrix3x2{})
								.Set("Shading.dm3x4", Ray::DMatrix3x4{})
								.Set("Shading.dm4", Ray::DMatrix4{})
								.Set("Shading.dm4x2", Ray::DMatrix4x2{})
								.Set("Shading.dm4x3", Ray::DMatrix4x3{})
								.Acquire();

			m_Pivot = Scene::Get().CreateEntity();
			m_Pivot.Emplace<Ray::Mesh>(Ray::Mesh::Triangle(material));
			m_Pivot.Emplace<Ray::MeshRenderer>(material);
		}

	private:
		virtual void OnEvent(Ray::CameraChangedEvent& e) override
		{
			m_Pivot.Get<Ray::Transform>().SetTranslation(Scene::Get().GetCamera().Pivot);
		}

	private:
		Ray::Entity m_Pivot;
	};

}  // namespace At0::Raditor


void SignalHandler(int signal)
{
	At0::Ray::Log::Critical("Signal {0} received", signal);
	At0::Ray::Log::Close();
}

int main()
{
	using namespace At0;

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
		Ray::Window::Get().SetTitle("Raditor");

		return Raditor::App{}.Run();
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