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

		IFileDialog* pFileSave = nullptr;
		NPE_THROW_GFX_EXCEPT(CoCreateInstance(
			CLSID_FileSaveDialog, 
			NULL,
			CLSCTX_ALL,
			IID_IFileSaveDialog,
			(void**)&pFileSave), 
			"Failed to create IFileOpenDialog");

		NPE_THROW_GFX_EXCEPT(pFileSave->SetDefaultExtension(L"dbs;txt"), "Failed to set default extension for IFileOpenDialog");
		NPE_THROW_GFX_EXCEPT(pFileSave->Show(NULL), "Failed to show IFileOpenDialog");
		
		IShellItem* pItem;
		NPE_THROW_GFX_EXCEPT(pFileSave->GetResult(&pItem), "Failed to get result from IFileOpenDialog");

		NPE_THROW_GFX_EXCEPT(pItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath), "Failed to get display name from IFileOpenDialog");
		Util::Release(&pItem);
		Util::Release(&pFileSave);

		return filePath;
	}
}



