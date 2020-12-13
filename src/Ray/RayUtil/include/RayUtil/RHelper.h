#pragma once

#include "RUBase.h"

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

	/// <summary>
	/// Allocates the console and sets std::cout output
	/// </summary>
	void RU_API ConsoleSetup();

	/// <summary>
	/// Frees the console
	/// </summary>
	void RU_API ConsoleShutdown();
}


