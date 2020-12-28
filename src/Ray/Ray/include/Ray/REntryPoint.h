#ifndef RAY_ENTRYPOINT_H
#define RAY_ENTRYPOINT_H


#include "Ray/RApplication.h"
#include "../../RayUtil/include/RayUtil/RTypeConvert.h"

#include <string>

namespace At0::Ray
{
	/// <summary>
	/// Function which needs to be defined by the client to create and destroy his application
	/// </summary>
	/// <param name="argc">Is the main function's argument count</param>
	/// <param name="argv">Are the console arguments passed to the program</param>
	int Awake(std::string commandLineArguments);
}



#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#include <sal.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int pCmdShow)
{
	std::string cmdLine = ::At0::Ray::Util::WideCharToMultiByte(pCmdLine);
	::At0::Ray::Application::SetCommandLineArguments(cmdLine);
	return ::At0::Ray::Awake(std::move(cmdLine));
}

#elif defined(__linux__)

int main(int argc, char** argv)
{

	std::string cmdLine = "";
	for (uint32_t i = 0; i < argc; ++i)
	{
		cmdLine += argv[i];
		if (i + 1 < argc)
			cmdLine += ' ';
	}

	::At0::Ray::Application::SetCommandLineArguments(cmdLine);
	return At0::Ray::Awake(std::move(cmdLine));
}

#endif // _WIN32

#else
#error "Application Entry Point file included twice."
#endif // RAY_ENTRYPOINT_H