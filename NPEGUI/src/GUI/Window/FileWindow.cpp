#include "pch.h"
#include "FileWindow.h"

#include "Util/Direct2D.h"
#include "Util/Exceptions.h"


/**
* QUESTION:
*	Use nullptr or NULL for Win32API types (HWND is typedef struct*   <-- so it's actually a pointer)
*/

namespace GUI
{
	std::wstring FileWindow::ShowSaveDialog(_In_ const wchar_t* title, _In_ FilterSpecs* rgSpecs, _In_ unsigned int numSpecs)
	{
		wchar_t* filePath;

		IFileSaveDialog* pFileSave = nullptr;
		NPE_THROW_WND_EXCEPT(CoCreateInstance(
			CLSID_FileSaveDialog,
			NULL,
			CLSCTX_ALL,
			IID_IFileSaveDialog,
			(void**)&pFileSave
		));

		NPE_THROW_WND_EXCEPT(pFileSave->SetFileTypes(numSpecs, (COMDLG_FILTERSPEC*)rgSpecs));
		NPE_THROW_WND_EXCEPT(pFileSave->SetTitle(title));

		HRESULT hr = pFileSave->Show(NULL);
		if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED))
		{
			Util::Release(&pFileSave);
			return L"";
		}
		else if (hr == E_FAIL)
		{
			throw NPE::WindowException(hr, __LINE__, __FILE__);
		}

		IShellItem* pItem;
		NPE_THROW_WND_EXCEPT(pFileSave->GetResult(&pItem));

		if (!pItem)
		{
			Util::Release(&pFileSave);
			return L"";
		}

		NPE_THROW_WND_EXCEPT(pItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath));
		Util::Release(&pItem);
		Util::Release(&pFileSave);

		return filePath;
	}
	
	std::wstring FileWindow::ShowOpenDialog(_In_ const wchar_t* title, _In_ FilterSpecs* rgSpecs, _In_ unsigned int numSpecs)
	{
		wchar_t* filePath;

		IFileOpenDialog* pFileOpen = nullptr;
		NPE_THROW_WND_EXCEPT(CoCreateInstance(
			CLSID_FileOpenDialog,
			NULL,
			CLSCTX_ALL,
			IID_IFileOpenDialog,
			(void**)&pFileOpen
		));

		NPE_THROW_WND_EXCEPT(pFileOpen->SetFileTypes(numSpecs, (COMDLG_FILTERSPEC*)rgSpecs));
		NPE_THROW_WND_EXCEPT(pFileOpen->SetTitle(title));

		HRESULT hr = pFileOpen->Show(NULL);
		if (hr == HRESULT_FROM_WIN32(ERROR_CANCELLED))
		{
			Util::Release(&pFileOpen);
			return L"";
		}
		else if (hr == E_FAIL)
		{
			throw NPE::WindowException(hr, __LINE__, __FILE__);
		}

		IShellItem* pItem;
		NPE_THROW_WND_EXCEPT(pFileOpen->GetResult(&pItem));

		if (!pItem)
		{
			Util::Release(&pFileOpen);
			return L"";
		}

		NPE_THROW_WND_EXCEPT(pItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath));
		Util::Release(&pItem);
		Util::Release(&pFileOpen);

		return filePath;
	}
}



