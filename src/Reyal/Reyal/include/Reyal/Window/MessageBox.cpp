#include "MessageBox.h"

namespace At0::Reyal
{
	ZMessageBox::ZMessageBox(const std::string_view title, const std::string_view text, Button buttons, Icon icon, Flags flags)
	{
#ifdef _WIN32

		long types = 0;
		switch (buttons)
		{
		case Button::Ok:
		{
			types |= MB_OK;
			break;
		}
		case Button::OkCancel:
		{
			types |= MB_OKCANCEL;
			break;
		}
		}

		switch (icon)
		{
		case Icon::Error:
		{
			types |= MB_ICONERROR;
			break;
		}
		}

		switch (flags)
		{
		case Flags::DefaultDesktop:
		{
			types |= MB_DEFAULT_DESKTOP_ONLY;
			break;
		}
		}

		MessageBoxA(NULL, text.data(), title.data(), types);

#elif defined(__linux__)
#endif
	}
}


