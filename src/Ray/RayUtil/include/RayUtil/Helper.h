#pragma once

#include "RlUBase.h"

#if defined(_WIN32) && !defined(NDEBUG)
#include <Windows.h>
#endif

#include <memory>
#include <signal.h>


namespace At0::Ray
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename... Args>
	Scope<T> MakeScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	Ref<T> MakeRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

namespace At0::Util
{
	template<typename F>
	static void SetSignals(F&& func)
	{
		signal(SIGABRT, func);
		signal(SIGFPE, func);
		signal(SIGILL, func);
		signal(SIGINT, func);
		signal(SIGSEGV, func);
		signal(SIGTERM, func);
	}

	static void ConsoleSetup()
	{
		// Allocates Console and sets std::cout output in debug builds
#if defined(_WIN32) && !defined(NDEBUG)
		{
			AllocConsole();
			FILE* file;
			freopen_s(&file, "CONOUT$", "w", stdout);
		}
#endif
	}

	static void ConsoleShutdown()
	{
#if defined(_WIN32) && !defined(NDEBUG)
		{
			FreeConsole();
		}
#endif
	}
}


