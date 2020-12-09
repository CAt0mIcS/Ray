#pragma once

#include <string>
#include <functional>

#include <Ray/Layers/Layer.h>


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
		void Start(const std::string_view searchPath, const std::function<void(Ray::Layer*)>& onNewLayer);

		/// <summary>
		/// ExtensionLoader Deconstructor, frees loaded libraries
		/// </summary>
		~ExtensionLoader();

	private:
		std::vector<void*> m_LibHandles;
	};
}


