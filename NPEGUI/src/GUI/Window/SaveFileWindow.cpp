#include "pch.h"
#include "SaveFileWindow.h"

#include "Util/Direct2D.h"
#include "Util/Exceptions.h"


/**
* QUESTION:
*	Use nullptr or NULL for Win32API types (HWND is typedef struct*)
*/

namespace GUI
{
	std::wstring SaveFileWindow::Show(_In_ const wchar_t* title, _In_ const wchar_t* filter)
	{
		PWSTR filePath;

		IFileDialog* pFileOpen = nullptr;
		NPE_THROW_GFX_EXCEPT(CoCreateInstance(
			CLSID_FileOpenDialog, 
			NULL,
			CLSCTX_ALL,
			IID_IFileOpenDialog,
			(void**)&pFileOpen), 
			"Failed to create IFileOpenDialog");

		NPE_THROW_GFX_EXCEPT(pFileOpen->SetDefaultExtension(L"dbs;txt"), "Failed to set default extension for IFileOpenDialog");
		NPE_THROW_GFX_EXCEPT(pFileOpen->Show(NULL), "Failed to show IFileOpenDialog");
		
		IShellItem* pItem;
		NPE_THROW_GFX_EXCEPT(pFileOpen->GetResult(&pItem), "Failed to get result from IFileOpenDialog");

		NPE_THROW_GFX_EXCEPT(pItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath), "Failed to get display name from IFileOpenDialog");
		Util::Release(&pItem);
		Util::Release(&pFileOpen);

		return filePath;
	}
}



