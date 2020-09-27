#pragma once

#include <functional>
#include <vector>

#include "BaseWindow.h"
#include "GUI/Controls/Control.h"
#include "GUI/Util/Timer.h"


namespace GUI
{
	class Event;

	/**
	* Event filter function type
	* 
	* @param watched is the object that received the event
	* @param e is the received event
	* @returns true if the event was handled, false otherwise
	*/
	using EventCallbackFn = std::function<bool(GUI::Control* watched, GUI::Event& e)>;

	class GUI_API MainWindow : public BaseWindow<MainWindow>, public Control
	{
	public:
		/**
		* MainWindow constructor
		*/
		MainWindow();

		/**
		* Renders the window background
		* 
		* @returns always true
		*/
		virtual bool Render() override;

		/**
		* All events of the specific control will be dispatched to this function
		*
		* @param e is the received event
		* @returns true if the event was handled, else false and the event will be dispatched to the client
		*/
		virtual bool OnEvent(_In_ Event& e) override;

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
		LRESULT HandleMessage(_In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) override;

		/**
		* Getter for all controls in the window
		* 
		* @returns vector of all controls
		*/
		std::vector<Control*>& GetControls() { return m_Children; }


		/**
		* QUESTION:
		*	Should I do it like this or have it return a control or have the argument control be a template argument T
		*/

		/**
		* Adds a Control to the window
		* 
		* @returns the added Control
		*/
		template<typename T>
		T* AddControl(_In_ Control* control) { return (T*)m_Children.emplace_back(control); }

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

		/**
		* Sets the event callback
		* 
		* @tparam F is any callable and to GUI::EventCallbackFn castable type
		* @param func is the function to set the event callback to
		*/
		template<typename F>
		void SetEventCallback(_In_ F&& func) { m_EventCallbackFn = func; }

		/**
		* Creates a new timer
		* 
		* @param time is the time in milliseconds the timer runs
		* @param repeats is true if the timer should repeat
		*/
		void CreateTimer(_In_ unsigned int time, _In_ bool repeats);

		/**
		* Sets the HWND window title
		* 
		* @param title is the new title
		*/
		void SetTitle(const std::wstring& title);

		/**
		* Invalidates the window and thus sends WM_PAINT message
		* 
		* @param rc is the rect to redraw, nullptr to redraw the entire window
		* @param erase specifies whether the background within the update region should be erased
		*/
		void PostRedraw(const RECT* const rc = nullptr, BOOL erase = TRUE) { InvalidateRect(m_hWnd, rc, erase); UpdateWindow(m_hWnd); }

	private:
		/**
		* Receives all events, finds the event receiver and dispatches the event to first the receiver and then the user
		* 
		* @param e is the received event from window procedure
		* @returns true if the event was handled, false otherwise
		*/
		bool DispatchEvent(_In_ Event& e);

		/**
		* Handles WM_TIMER message
		* 
		* @param id is the wParam (the timer id) of the WM_TIMER message
		* @returns true if the message was handled
		*/
		bool HandleTimer(_In_ unsigned int id);

	private:
		EventCallbackFn m_EventCallbackFn;
		std::vector<Timer> m_Timers;

	};

}



