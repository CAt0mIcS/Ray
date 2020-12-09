#ifndef RL_ENTRYPOINT_H
#define RL_ENTRYPOINT_H


#include "Ray/Application.h"
#include <string>

extern void At0::Ray::Awake(int argc, char** argv);


#ifdef _WIN32

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#include <sal.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PWSTR pCmdLine, _In_ int pCmdShow)
{
	// TODO: Command Line Arguments
	At0::Ray::Awake(0, nullptr);
}

#elif defined(__linux__)


int main(int argc, char** argv)
{
	// TODO: Command Line Arguments
	At0::Ray::Awake(argc, argv);
}

#endif // _WIN32

#else
	#error "Application Entry Point file included twice."
#endif // RL_ENTRYPOINT_H