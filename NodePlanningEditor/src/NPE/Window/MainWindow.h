#pragma once

#include "pch.h"
#include "BaseWindow.h"

#include "NPE/Handlers/Mouse.h"
#include "NPE/Handlers/Keyboard.h"

#include "NPE/Controls/Node.h"

#include "NPE/Graphics/Renderer2D.h"

namespace NPE
{
	class MainWindow : public BaseWindow<MainWindow>
	{
		friend class Renderer2D;
	public:
		MainWindow(unsigned short width, unsigned short height, PCWSTR name);

		template<typename F>
		int ProcessMessage(F&& func);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		std::vector<NodeRect>& GetControls() { return m_Nodes; }
		Renderer2D& Get2DRenderer() { return m_Renderer2D; }

	public:
		Mouse Mouse;
		Keyboard Keyboard;

	private:
		void Paint(HDC hDC, RECT* rcDirty, BOOL bErase);
		Renderer2D m_Renderer2D;
		std::vector<NodeRect> m_Nodes;

		//std::vector<Node> m_Controls;
	};


	template<typename F>
	inline int MainWindow::ProcessMessage(F&& func)
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



