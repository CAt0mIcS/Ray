#pragma once

#include "BaseWindow.h"
#include "GUI/Controls/Control.h"
#include "GUI/Util/Timer.h"

#include <functional>
#include <vector>
#include <type_traits>

//TODO: Add to pch
#include <Uxtheme.h>


namespace GUI
{
	class Event;
	class PaintEvent;

	/// <summary>
	/// Event filter function type
	/// <param name="watched">Is the Control that received the event</param>
	/// <param name="e">Is the received event</param>
	/// <returns>True if the event was handled, false otherwise</returns>
	/// </summary>
	using EventCallbackFn = std::function<bool(GUI::Control* watched, GUI::Event& e)>;

	/// <summary>
	/// MainWindow class
	/// </summary>
	class GUI_API MainWindow : public BaseWindow<MainWindow>, public Control
	{
	public:
		/// <summary>
		/// MainWindow Constructor
		/// </summary>
		MainWindow();

		/// <summary>
		/// Renders the entire window
		/// </summary>
		/// <returns>Always returns true</returns>
		virtual bool Render() override;

		/// <summary>
		/// All events of a specific Control will be dispatched to this function
		/// </summary>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, else false and the event will be dispatched to the client</returns>
		virtual bool OnEvent(_In_ Event& e) override;

		/// <summary>
		/// Enters the message loop
		/// </summary>
		/// <returns>The exit code when quitting the application</returns>
		static int ProcessMessage();

		/// <summary>
		/// Handles all window messages
		/// </summary>
		/// <param name="uMsg">Is the message code</param>
		/// <param name="wParam">Is an additional parameter</param>
		/// <param name="lParam">Is an additional parameter</param>
		/// <returns>LERSULT code</returns>
		LRESULT HandleMessage(_In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) override;

		/// <summary>
		/// Getter for all Controls in the window
		/// </summary>
		/// <returns>List of Controls in the window</returns>
		Control::ContainerType& GetControls() { return m_Children; }


		/**
		* QUESTION:
		*	Should I do it like this or have it return a Control or have the argument Control be a template argument T
		*	Should I use std::enable_if_t or static_assert
		*/

		/// <summary>
		/// Adds a Control to the window
		/// </summary>
		/// <typeparam name="T">Is any to Control castable object</typeparam>
		/// <param name="control">Is the Control to add</param>
		/// <returns>The added Control</returns>
		template<typename T, typename = std::enable_if_t<std::is_convertible<T*, Control*>::value>>
		T* AddControl(_In_ Control* control) { return (T*)m_Children.emplace_back(control); }

		/// <summary>
		/// Getter for client rect
		/// </summary>
		/// <returns>The client rect</returns>
		RECT GetClientRect() const;

		/// <summary>
		/// Getter for the window rrect
		/// </summary>
		/// <returns>The window rect</returns>
		RECT GetWindowRect() const;

		/// <summary>
		/// Sets the event callback function
		/// </summary>
		/// <typeparam name="F">Is any callable and to GUI::EventCallbackFn castable type</typeparam>
		/// <param name="func">Is the function to set the event callback to</param>
		template<typename F, typename = std::enable_if_t<std::is_invocable<F, Control*, Event&>::value>>
		void SetEventCallback(_In_ F&& func) { m_EventCallbackFn = func; }

		/// <summary>
		/// Creates a new Timer
		/// </summary>
		/// <param name="time">Is the time in milliseconds the Timer should run</param>
		/// <param name="repeats">Is true if the timer should repeat</param>
		void CreateTimer(_In_ unsigned int time, _In_ bool repeats);

		/// <summary>
		/// Sets the window title
		/// </summary>
		/// <param name="title">Is the new title of the window</param>
		void SetTitle(const std::wstring& title);

		/// <summary>
		/// Post a redraw message to the window, invalidates it and then calls UpdateWindow, thus sending WM_PAINT message
		/// </summary>
		/// <param name="rc">Is the rect to redraw, null to redraw the entire window</param>
		/// <param name="erase">Specifies whether the background within the update region should be erased</param>
		void PostRedraw(const RECT* const rc = nullptr, BOOL erase = TRUE) const { InvalidateRect(m_hWnd, rc, erase); UpdateWindow(m_hWnd); }

		/// <summary>
		/// MainWindow Deconstructor
		/// </summary>
		~MainWindow();

	private:
		
		/// <summary>
		/// Receives all events, finds the event receiver and dispatches the event to first the receiver and then the user
		/// </summary>
		/// <param name="e">Is the received event from window procedure</param>
		/// <returns>True if the event was handled, false otherwise</returns>
		bool DispatchEvent(_In_ Event& e);

		/// <summary>
		/// Receives all paint events, finds the control that needs to be redrawn and dispatches the event to the user
		/// </summary>
		/// <param name="e">Is the received event from WM_PAINT</param>
		/// <returns>True if the event was handled, false otherwise</returns>
		bool DispatchPaintEvent(_In_ PaintEvent& e);

		/// <summary>
		/// Handles WM_TIMER message
		/// </summary>
		/// <param name="id">Is the wParam (the Timer Id) of the WM_TIMER message</param>
		/// <returns>True if the message was handled, false otherwise</returns>
		bool HandleTimer(_In_ unsigned int id);

	private:
		/// <summary>
		/// Event callback function, set with MainWindow::SetEventCallback, 
		/// NPE::Application::Application calls InstallEventFilter (function in GUI::GUIApplication),
		/// which calls MainWindow::SetEventCallback
		/// </summary>
		EventCallbackFn m_EventCallbackFn;

		/// <summary>
		/// List of all Timers
		/// </summary>
		std::vector<Timer> m_Timers;
	};

}



