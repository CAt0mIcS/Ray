#pragma once

#include "pch.h"
#include "BaseWindow.h"

#include "NPE/Handlers/Mouse.h"
#include "NPE/Handlers/Keyboard.h"

#include "NPE/Controls/Node.h"


namespace NPE
{
	class MainWindow : public BaseWindow<MainWindow>
	{
	public:
		MainWindow(unsigned short width, unsigned short height, PCWSTR name);

		template<typename F>
		static int ProcessMessage(F&& func);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		std::vector<Node>& GetNodes() { return m_Nodes; }

	public:
		Mouse Mouse;
		Keyboard Keyboard;

	private:
		void Paint(HDC hDC, RECT* rcDirty, BOOL bErase);

		std::vector<Node> m_Nodes;
	};


	template<typename F>
	int MainWindow::ProcessMessage(F&& func)
	{
		MSG msg{};
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			func();

		}
		return (int)msg.wParam;
	}

}



