#pragma once

#include "RUBase.h"

#include <memory>
#include <signal.h>
#include <string>


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

namespace At0::Ray::Util
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
	void RU_API AllocateConsole();

	/// <summary>
	/// Frees the console
	/// </summary>
	void RU_API DeallocateConsole();

	/// <summary>
	/// Checks if a string ends with another string
	/// </summary>
	/// <param name="str">Is the full string to check</param>
	/// <param name="end">Is the supposed end of the full string</param>
	/// <returns>True if str ends with end, false otherwise</returns>
	bool RU_API EndsWith(std::string_view str, std::string_view end);
}


