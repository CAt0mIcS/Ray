#pragma once

#include "GUIBase.h"


namespace GUI
{
	class GUI_API SaveFileWindow
	{
	public:
		SaveFileWindow() = default;

		/**
		* Shows the Window
		* 
		* @param title is the title of the window
		* @param filter are file extensions the user will be able to select (e.g. "Text Files\0*.txt\0Any File\0*.*\0")
		* @returns the path to the file
		*/
		std::wstring Show(
			_In_ const wchar_t* title,
			_In_ const wchar_t* filter
		);
	};
}


