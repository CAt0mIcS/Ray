#ifndef ZL_EXTENSION_LOADER_H
#define ZL_EXTENSION_LOADER_H

#include <string>


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
		void Start(const std::string_view searchPath);
	};
}



#endif // ZL_EXTENSION_LOADER_H