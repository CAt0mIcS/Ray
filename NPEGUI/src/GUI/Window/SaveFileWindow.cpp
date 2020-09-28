#include "pch.h"
#include "SaveFileWindow.h"


namespace GUI
{
	std::wstring SaveFileWindow::Show(_In_ const wchar_t* title, _In_ const wchar_t* filter)
	{

		std::wstring path = L"";
		path.reserve(MAX_PATH);

		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));

		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = filter;
		ofn.lpstrFile = (wchar_t*)path.c_str();
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = title;
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn))
		{
			wprintf(L"You chose the file \"{%s}\"", path.c_str());
		}
		else
		{
			switch (CommDlgExtendedError())
			{
			case CDERR_DIALOGFAILURE: printf("CDERR_DIALOGFAILURE\n");   break;
			case CDERR_FINDRESFAILURE: printf("CDERR_FINDRESFAILURE\n");  break;
			case CDERR_INITIALIZATION: printf("CDERR_INITIALIZATION\n");  break;
			case CDERR_LOADRESFAILURE: printf("CDERR_LOADRESFAILURE\n");  break;
			case CDERR_LOADSTRFAILURE: printf("CDERR_LOADSTRFAILURE\n");  break;
			case CDERR_LOCKRESFAILURE: printf("CDERR_LOCKRESFAILURE\n");  break;
			case CDERR_MEMALLOCFAILURE: printf("CDERR_MEMALLOCFAILURE\n"); break;
			case CDERR_MEMLOCKFAILURE: printf("CDERR_MEMLOCKFAILURE\n");  break;
			case CDERR_NOHINSTANCE: printf("CDERR_NOHINSTANCE\n");     break;
			case CDERR_NOHOOK: printf("CDERR_NOHOOK\n");          break;
			case CDERR_NOTEMPLATE: printf("CDERR_NOTEMPLATE\n");      break;
			case CDERR_STRUCTSIZE: printf("CDERR_STRUCTSIZE\n");      break;
			case FNERR_BUFFERTOOSMALL: printf("FNERR_BUFFERTOOSMALL\n");  break;
			case FNERR_INVALIDFILENAME: printf("FNERR_INVALIDFILENAME\n"); break;
			case FNERR_SUBCLASSFAILURE: printf("FNERR_SUBCLASSFAILURE\n"); break;
			default: printf("You canceled"); break;
			}
		}
		return path;
	}
}



