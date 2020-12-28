#include "pch.h"
#include "Sandbox.h"

#include <Ray/Ray.h>

#include "ExtensionLoader/ExtensionLoader.h"

template<typename>
struct IsWStringCompatible
{
	static constexpr bool Value = false;
};

template<>
struct IsWStringCompatible<std::wstring>
{
	static constexpr bool Value = true;
};

template<>
struct IsWStringCompatible<wchar_t*>
{
	static constexpr bool Value = true;
};

template<>
struct IsWStringCompatible<const wchar_t*>
{
	static constexpr bool Value = true;
};

template<>
struct IsWStringCompatible<wchar_t>
{
	static constexpr bool Value = true;
};

template<>
struct IsWStringCompatible<std::wstring_view>
{
	static constexpr bool Value = true;
};


template<typename, typename = void>
struct IsWStringConvertible : std::false_type {};

template<typename T>
struct IsWStringConvertible<T, std::void_t<decltype(&T::operator std::wstring)>> : std::true_type {};


template<typename T>
using RemoveKeywords = std::remove_cv_t<std::remove_reference_t<T>>;


#include <sstream>

template<typename, typename = void>
struct IsConvertible : std::false_type {};

template<typename T>
struct IsConvertible<T, std::void_t<decltype(std::declval<std::wostringstream> << std::declval<T>())>> : std::true_type {};



namespace At0::Zeal
{
	Sandbox::Sandbox(std::string_view commandLine)
		: Application(commandLine)
	{
		//RAY_PROFILE_FUNCTION();
		Ray::Util::AllocateConsole();

		class Data
		{
		public:
			Data(float x) : x(x) {}

			operator std::wstring() const
			{
				return std::to_wstring(x);
			}

		private:
			float x;
		};
		const Data data(32.342f);

		std::wstring str = L"32";
		std::wostringstream oss;
		oss << str;

		//if constexpr (IsWStringCompatible<RemoveKeywords<decltype(str)>>::Value || IsWStringConvertible<decltype(data)>::value)
		//{
		//	std::cout << "Compatible\n";
		//}
		//else
		//{
		//	std::cout << "Non-Compatible\n";
		//}

		if constexpr (IsConvertible<RemoveKeywords<decltype(str)>>::value)
		{
			std::cout << "Compatible\n";
		}
		else
		{
			std::cout << "Non-Compatible\n";
		}
		std::cin.get();
		exit(0);

		GetMainWindow().SetTitle("Zeal");
		GetMainWindow().Show();
		GetMainWindow().SetIcon("Resources/Icon.png");

		//Ray::Window& win = PushWindow(Ray::Window::Create("Win0", { 150, 150 }, { 960, 540 }));
		//win.SetTitle("Win0");
		//win.Show();

		//Ray::Window& win1 = PushWindow(Ray::Window::Create("Win1"));
		//win.SetTitle("Win1");
		//win.Show();

		//Ray::Window& win2 = PushWindow(Ray::Window::Create("Win2"));
		//win.SetTitle("Win2");
		//win.Show();

		//Ray::Window& win3 = PushWindow(Ray::Window::Create("Win3"));
		//win.SetTitle("Win3");
		//win.Show();

		//Ray::Window& win4 = PushWindow(Ray::Window::Create("Win4"));
		//win.SetTitle("Win4");
		//win.Show();

		m_LayerLoader.Start("Editors", [this](Ray::Layer* layer)
			{
				PushLayer(layer);
			}
		);

	}

	Sandbox::~Sandbox()
	{
		//RAY_PROFILE_FUNCTION();
	}

	void Sandbox::OnEvent(Ray::Widget* receiver, Ray::Event& e)
	{
		//RAY_PROFILE_FUNCTION();
	}

}


#include <Ray/REntryPoint.h>

#include <RayDebug/RInstrumentor.h>
#include <RayDebug/RLogger.h>
#include <RayUtil/RException.h>
#include <RayRender/RendererAPI.h>


///////////////////////////////////////////////////////////////////////////
////////// Called in EntryPoint.h /////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
int At0::Ray::Awake(std::string commandLineArguments)
{
	using namespace At0;

	try
	{
		RAY_LOG_BEGIN("../../Zeal.log", Log::LogLevel::Trace);

		RAY_PROFILE_BEGIN_SESSION("Startup", "../../Profiling/Profile-Startup.json");
		Ray::RendererAPI::SetAPI(Ray::RendererAPI::D3D11);
		Zeal::Sandbox* app = new Zeal::Sandbox(commandLineArguments);
		RAY_PROFILE_END_SESSION();

		RAY_PROFILE_BEGIN_SESSION("Runtime", "../../Profiling/Profile-Runtime.json");
		int exitCode = app->Run();
		RAY_PROFILE_END_SESSION();

		RAY_PROFILE_BEGIN_SESSION("Shutdown", "../../Profiling/Profile-Shutdown.json");
		delete app;
		RAY_PROFILE_END_SESSION();

		RAY_LOG_END();
		return exitCode;
	}
	catch (At0::Ray::Exception& e)
	{
		Ray::RMessageBox(e.GetType(), e.what(), Ray::RMessageBox::Button::Ok, Ray::RMessageBox::Icon::Error, Ray::RMessageBox::Flags::DefaultDesktop);
		RAY_LOG_CRITICAL("[Awake] Exception occured: {0}", e.what());
	}
	catch (std::exception& e)
	{
		Ray::RMessageBox("Standard Exception", e.what(), Ray::RMessageBox::Button::Ok, Ray::RMessageBox::Icon::Error, Ray::RMessageBox::Flags::DefaultDesktop);
		RAY_LOG_CRITICAL("[Awake] Exception occured: {0}", e.what());
	}
	catch (...)
	{
		Ray::RMessageBox("Unknown Exception", "An unknown exception occurred", Ray::RMessageBox::Button::Ok, Ray::RMessageBox::Icon::Error, Ray::RMessageBox::Flags::DefaultDesktop);
		RAY_LOG_CRITICAL("[Awake] Unknown Exception occured");
	}
	RAY_LOG_END();
	return -1;
}

