#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"

#include "NPE/Application.h"
#include "Util/Exceptions.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR nCmdLine, _In_ int nCmdShow)
{
	try
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		return NPE::Application{}.Run();
	}
	catch (NPE::Exception& e)
	{
		MessageBoxA(NULL, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (std::exception& e)
	{
		MessageBoxA(NULL, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(NULL, "Unknown Exception occured", "Unknown exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}
