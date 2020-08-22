#pragma once

#include "pch.h"
#include "BaseWindow.h"


namespace NPE
{
	class MainWindow : public BaseWindow<MainWindow>
	{
	public:
		MainWindow();

		void Show(_In_ int nCmdShow);

		void EnterMessageLoop();

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		void Paint(HDC hDC, RECT* rcDirty, BOOL bErase);
	};
}



