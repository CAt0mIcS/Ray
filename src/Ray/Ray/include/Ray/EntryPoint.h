#ifndef RAY_ENTRYPOINT_H
#define RAY_ENTRYPOINT_H


#include "Ray/Application.h"
#include <string>

namespace At0::Ray
{
	/// <summary>
	/// Function which needs to be defined by the client to create and destroy his application
	/// </summary>
	/// <param name="argc">Is the main function's argument count</param>
	/// <param name="argv">Are the console arguments passed to the program</param>
	int Awake(int argc, char** argv);
}


#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#include <sal.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int pCmdShow)
{
	// RAY_TODO: Command Line Arguments
	return At0::Ray::Awake(0, nullptr);
}

#elif defined(__linux__)


int main(int argc, char** argv)
{
	// RAY_TODO: Command Line Arguments
	return At0::Ray::Awake(argc, argv);
}

#endif // _WIN32

#else
	#error "Application Entry Point file included twice."
#endif // RAY_ENTRYPOINT_H