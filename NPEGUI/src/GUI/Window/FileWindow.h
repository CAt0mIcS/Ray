#pragma once

#include "GUIBase.h"


namespace GUI
{
	class GUI_API FileWindow
	{
	public:
		/// <summary>
		/// Defines specifications to implement a filter in the window
		/// </summary>
		struct FilterSpecs
		{
			/// <summary>
			/// Specifies any name of the file ("Text File")
			/// </summary>
			LPCWSTR pszName;

			/// <summary>
			/// Specifies the actual extension ("*.txt")
			/// </summary>
			LPCWSTR pszSpec;
		};

	public:
		/// <summary>
		/// Default FileWindow Constructor
		/// </summary>
		FileWindow() = default;

		/// <summary>
		/// Shows a Save Dialog Window
		/// </summary>
		/// <param name="title">Is the title of the window</param>
		/// <param name="rgSpecs">Is a list of FilterSpecs</param>
		/// <param name="numSpecs">Is the element count in rgSpecs</param>
		/// <returns></returns>
		std::wstring ShowSaveDialog(
			_In_ const wchar_t* title,
			_In_ FilterSpecs* rgSpecs,
			_In_ unsigned int numSpecs
		);

		/// <summary>
		/// Shows a Open Dialog Window
		/// </summary>
		/// <param name="title">Is the title of the window</param>
		/// <param name="rgSpecs">Is a list of FilterSpecs</param>
		/// <param name="numSpecs">Is the element count in rgSpecs</param>
		/// <returns></returns>
		std::wstring ShowOpenDialog(
			_In_ const wchar_t* title,
			_In_ FilterSpecs* rgSpecs,
			_In_ unsigned int numSpecs
		);
	};
}


