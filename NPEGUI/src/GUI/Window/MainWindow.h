#pragma once

#include <functional>
#include <vector>

#include "BaseWindow.h"

#include "GUI/Events/Event.h"

#include "GUI/Graphics/Renderer.h"
#include "GUI/Controls/Control.h"


namespace GUI
{
	class Button;

	class MainWindow : public BaseWindow<MainWindow>
	{
	public:
		/**
		* MainWindow constructor
		*/
		MainWindow();

		/**
		* Enters the message loop
		* 
		* @returns the exit code on exit
		*/
		static int ProcessMessage();

		/**
		* Handles all window messages
		*
		* @param uMsg is the window message
		* @param wParam is the wParam
		* @param lParam is the lParam
		*/
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		/**
		* Getter for all controls in the window
		* 
		* @returns vector of all controls
		*/
		std::vector<Control*>& GetControls() { return m_Controls; }

		/**
		* Adds a Control to the window
		* 
		* @returns the added Control
		*/
		Control* AddControl(Control* control) { return m_Controls.emplace_back(control); }

		/**
		* Getter for window position
		* 
		* @returns the window position
		*/
		Util::NPoint GetPos() const;

		/**
		* Getter for window size
		*
		* @returns the window size
		*/
		Util::NSize GetSize() const;

		/**
		* Getter for window rect
		*
		* @returns the window rect
		*/
		RECT GetRect() const;

	private:
		std::function<void(const Event& e)> m_EventCallback;
		std::vector<Control*> m_Controls;

	};

}



