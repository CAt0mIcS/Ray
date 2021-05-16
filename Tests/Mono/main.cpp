#include <Core/REngine.h>
#include <Utils/RLogger.h>
#include <Devices/RWindow.h>
#include <Graphics/RGraphics.h>

#include <UI/RButton.h>

#include <Components/RMesh.h>
#include <Components/RMeshRenderer.h>
#include <Components/RTransform.h>
#include <Components/RSkybox.h>

#include <Graphics/Images/RTexture2D.h>
#include <Graphics/Images/RTextureCubemap.h>
#include <Graphics/Pipelines/RGraphicsPipeline.h>
#include <Utils/RException.h>
#include <Core/RDynamicVertex.h>

#include <Shading/Phong/RPhongMaterial.h>
#include <Shading/Flat/RFlatColorMaterial.h>

#include <Scene/RScene.h>
#include <Scene/RCamera.h>

#include <signal.h>
#include <random>
#include <filesystem>

#include <UI/RImGui.h>
#include <../../Extern/imgui/imgui.h>


#include <mono/jit/jit.h>
#include <mono/metadata/object.h>
#include <mono/metadata/environment.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>


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


void RunStaticMethod(MonoDomain* domain, MonoImage* image, const char* methodDesc)
{
	MonoMethodDesc* TypeMethodDesc;
	TypeMethodDesc = mono_method_desc_new(methodDesc, NULL);
	if (!TypeMethodDesc)
		std::cout << "mono_method_desc_new failed" << std::endl;

	// Search the method in the image
	MonoMethod* method;
	method = mono_method_desc_search_in_image(TypeMethodDesc, image);
	if (!method)
		std::cout << "mono_method_desc_search_in_image failed" << std::endl;

	// run the method
	std::cout << "Running the static method: " << methodDesc << std::endl;
	mono_runtime_invoke(method, nullptr, nullptr, nullptr);
}


class App : public Ray::Engine
{
public:
	App()
	{
		Ray::Scene::Create<Scene>();

		// Initialize mono

		std::string assemblyDir = "Tests/Mono/CSharp/";

#ifdef _WIN32
		std::string command = Ray::String::Serialize(
			"\"C:/Program Files/Mono/bin/mcs\" {0}Dog.cs -target:library", assemblyDir);
#elif defined(__linux__)
		std::string command = Ray::String::Serialize("mcs {0}Dog.cs -target:library", assemblyDir);
#endif

		// Compile the script
		system(command.c_str());

#ifdef _WIN32
		mono_set_dirs("C:\\Program Files\\Mono\\lib", "C:\\Program Files\\Mono\\etc");
#endif

		/*
		 * Load the default Mono configuration file, this is needed
		 * if you are planning on using the dllmaps defined on the
		 * system configuration
		 */
		mono_config_parse(NULL);

		MonoDomain* domain =
			mono_jit_init(Ray::String::Serialize("{0}Dog.dll", assemblyDir).c_str());

		MonoAssembly* assembly;

		assembly = mono_domain_assembly_open(
			domain, Ray::String::Serialize("{0}Dog.dll", assemblyDir).c_str());
		if (!assembly)
			exit(2);

		MonoImage* image;
		image = mono_assembly_get_image(assembly);
		if (!image)
			std::cout << "mono_assembly_get_image failed" << std::endl;

		RunStaticMethod(domain, image, "Dog::Type()");

		mono_jit_cleanup(domain);
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
