#include "Rupch.h"
#include "RHelper.h"

#include <assert.h>


namespace At0::Ray::Util
{
	void AllocateConsole()
	{
#if defined(_WIN32)
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		freopen("CONNIN&", "r", stdin);
#endif
	}

	void ConsoleShutdown()
	{
#if defined(_WIN32)
		FreeConsole();
#endif
	}

	bool EndsWith(std::string_view str, std::string_view end)
	{
		assert(str.size() > end.size() && "[Util::EndsWith] The supposed end of the string is larger than it.");
		return str.substr(str.size() - end.size(), str.size()) == end;
	}
}

