#ifdef _DEBUG
	#define _CRT_SECURE_NO_WARNINGS
#endif
#include "pch.h"

#include "NPE/Application.h"
#include "Util/Exceptions.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR nCmdLine, _In_ int nCmdShow)
{
	try
	{
		/**
		* QUESTION:
		*	Should I indent #ifdef... like this or let VS position them to the left
		*	Should I keep error handling in release builds
		*/
		#ifdef _DEBUG
			AllocConsole();
			freopen("CONOUT$", "w", stdout);
		#endif

		return NPE::Application{}.Run();
	}
	catch (NPE::Exception& e)
	{
		MessageBoxW(NULL, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
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
