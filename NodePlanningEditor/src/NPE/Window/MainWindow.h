#pragma once

#include "pch.h"
#include "BaseWindow.h"

#include "NPE/Controls/Node.h"
#include "NPE/Handlers/Event.h"


namespace NPE
{
	class MainWindow : public BaseWindow<MainWindow>
	{
		friend class Renderer2D;
	public:
		MainWindow(unsigned short width, unsigned short height, PCWSTR name, std::function<void(const Event&)> eventFn);

		int ProcessMessage();

		void Update(const RECT* rc = nullptr, BOOL bErase = FALSE);

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		std::vector<Node>& GetControls() { return m_Controls; }
	
	public:

	private:
		void Paint(HDC hDC, RECT* rcDirty, BOOL bErase);

		struct WindowData
		{
			//unsigned int x, y, width, height;
			NPoint pos;
			NSize size;
			std::function<void(const Event& e)> EventCallback;
		};
		WindowData m_Data;

		std::vector<Node> m_Controls;
	};

}



