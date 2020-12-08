#pragma once

#include <Reyal/Application.h>
#include <Reyal/Window/Window.h>

#include "ExtensionLoader/ExtensionLoader.h"

namespace At0::Zeal
{

	class Sandbox : public Reyal::Application
	{
	public:
		/// <summary>
		/// Sandbox Constructor
		/// </summary>
		Sandbox(const std::string_view commandLine);

		/// <summary>
		/// Sandbox Deconstructor
		/// </summary>
		~Sandbox();


		///TODO: Currently not used! Could be used for WindowCloseEvent to check if some not MainWindow was closed which should close the entire application

		/// <summary>
		/// Receives all window events (WindowResize, WindowClose, WindowPaint)
		/// </summary>
		/// <param name="receiver">Is the window which receives the event</param>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, false otherwise</returns>
		virtual void OnEvent(Reyal::Widget* receiver, Reyal::Event& e);

	private:
		ExtensionLoader m_LayerLoader;
	};
	
}


