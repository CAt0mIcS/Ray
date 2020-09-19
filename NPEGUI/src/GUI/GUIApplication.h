#pragma once

#include "GUIBase.h"
#include "Window/MainWindow.h"


namespace GUI
{
	class GUI_API GUIApplication
	{
	public:
		/**
		* Adds a function to dispatch all events to
		* 
		* @tparam F is any callable and to GUI::EventCallbackFn castable type
		* @param func is the function which will be called when any event is received
		*/
		template<typename F>
		void InstallEventFilter(_In_ F&& func);

		/**
		* Start the application loop
		* 
		* @returns the exit code on program termination
		*/
		int Run();

		/**
		* Getter for main window
		* 
		* @returns the main window of the application
		*/
		MainWindow& GetWindow() { return m_Window; }

		/**
		* Getter for main window
		*
		* @returns the main window of the application
		*/
		const MainWindow& GetWindow() const { return m_Window; }

	protected:
		/**
		* GUIApplication constructor
		*/
		GUIApplication();

	protected:
		MainWindow m_Window;
	};


	template<typename F>
	inline void GUIApplication::InstallEventFilter(_In_ F&& func)
	{
		m_Window.SetEventCallback(func);
	}
}



