#pragma once

#include "GUIBase.h"


namespace GUI
{
	class GUI_API FileWindow
	{
	public:
		struct FilterSpecs
		{
			LPCWSTR pszName;
			LPCWSTR pszSpec;
		};

	public:
		FileWindow() = default;

		/**
		* Shows the Window
		* 
		* @param title is the title of the window
		* @param filter are file extensions the user will be able to select (e.g. "Text Files\0*.txt\0Any File\0*.*\0")
		* @returns the path to the file
		*/
		std::wstring ShowSaveDialog(
			_In_ const wchar_t* title,
			_In_ FilterSpecs rgSpecs[],
			_In_ unsigned int numSpecs
		);

		std::wstring ShowOpenDialog(
			_In_ const wchar_t* title,
			_In_ FilterSpecs rgSpecs[],
			_In_ unsigned int numSpecs
		);


	private:
	};
}


