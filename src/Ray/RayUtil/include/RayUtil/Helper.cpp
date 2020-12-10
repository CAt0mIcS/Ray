#include "utpch.h"
#include "Helper.h"


namespace At0::Util
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

