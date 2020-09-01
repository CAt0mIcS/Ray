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

		static std::optional<int> ProcessMessage();

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	public:
		Mouse mouse;
		Keyboard keyboard;

	private:
		void Paint(HDC hDC, RECT* rcDirty, BOOL bErase);

		std::vector<HWND> m_Controls;
		std::vector<Node> m_Nodes;

		NPoint m_MousePos;
	};
}



