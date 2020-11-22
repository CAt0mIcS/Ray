#pragma once

#include <string>
#include <functional>

#include <Reyal/Layers/Layer.h>


namespace At0::Zeal
{
	class ExtensionLoader
	{
	public:
		ExtensionLoader() = default;

		/// <summary>
		/// Loops through all .dll/.so files in a directory and loads them
		/// </summary>
		/// <param name="searchPath">Is the path to find all .dll files</param>
		void Start(const std::string_view searchPath, const std::function<void(Reyal::Layer*)>& onNewLayer);
	};
}


