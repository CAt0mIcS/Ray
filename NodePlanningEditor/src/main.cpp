#include "pch.h"

#include "NPE/Application.h"
#include "Util/Exceptions.h"

#include "Util/Debug/Logger.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR nCmdLine, _In_ int nCmdShow)
{
	try
	{
		Util::Logger::Init("NPE.log");

		/**
		* QUESTION:
		*	Should I indent #ifdef... like this or let VS position them to the left
		*	Should I keep error handling in release builds
		*/
		#ifdef _DEBUG
			AllocConsole();
			if (!freopen("CONOUT$", "w", stdout))
				NPE_LOG("Failed to open debug console");
		#endif

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

	/**
	* QUESTION:
	*	Should I return errno or get a exit code from the exception (<-- how?) or just return 1 or -1?
	*/
	return errno;
}
