#pragma once

#include <functional>
#include <vector>

#include "BaseWindow.h"

#include "NPE/Handlers/Event.h"

#include "NPE/Graphics/Renderer2D.h"
#include "NPE/Controls/Control.h"


namespace NPE
{
	using WindowPaintFn = std::function<void(HDC, RECT*)>;

	class MainWindow : public BaseWindow<MainWindow>
	{
	public:
		MainWindow(const NSize& size, PCWSTR name, std::function<void(const Event&)> eventFn);

		int ProcessMessage();

		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
		std::vector<Control*>& GetControls() { return m_Controls; }

		Control* AddControl(Control* control) { return m_Controls.emplace_back(control); }

		NPoint GetPos() const;
		NSize GetSize() const;
		RECT GetRect() const;

	public:
		Renderer2D Renderer2D;

	private:
		std::function<void(const Event& e)> m_EventCallback;
		std::vector<Control*> m_Controls;
	};

}



