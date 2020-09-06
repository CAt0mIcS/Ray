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

	class Button;

	class MainWindow : public BaseWindow<MainWindow>
	{
	public:
		/**
		* MainWindow constructor
		* 
		* @param size is the size of the window
		* @param name is the window name
		* @param eventFn is a function which should receive all events
		*/
		MainWindow(const NSize& size, PCWSTR name, std::function<void(const Event&)> eventFn);

		/**
		* Enters the message loop
		* 
		* @returns the exit code on exit
		*/
		int ProcessMessage();

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
		NPoint GetPos() const;

		/**
		* Getter for window size
		*
		* @returns the window size
		*/
		NSize GetSize() const;

		/**
		* Getter for window rect
		*
		* @returns the window rect
		*/
		RECT GetRect() const;

		/**
		* Adds a line between two buttons
		* 
		* @param line is a pair of two buttons to connect
		*/
		void AddLine(std::pair<Button*, Button*> line) { m_LineCons.emplace_back(line); }
		
		/**
		* Getter for all lines
		* 
		* @returns all lines
		*/
		std::vector<std::pair<Button*, Button*>>& GetLines() { return m_LineCons; }

		/**
		* Renders all lines in window
		* 
		* @warning function does not call BeginDraw or EndDraw
		*/
		void RenderLines();

	public:
		Renderer2D Renderer2D;

	private:

	private:
		std::function<void(const Event& e)> m_EventCallback;
		std::vector<Control*> m_Controls;

		std::vector<std::pair<Button*, Button*>> m_LineCons;
	};

}



