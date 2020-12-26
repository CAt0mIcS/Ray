#include "Rupch.h"
#include "RHelper.h"


namespace At0::Ray::Util
{
	void ConsoleSetup()
	{
#if defined(_WIN32)
		AllocConsole();
		FILE* file;
		freopen_s(&file, "CONOUT$", "w", stdout);
#endif
	}

	void ConsoleShutdown()
	{
#if defined(_WIN32)
		FreeConsole();
#endif
	}
}

