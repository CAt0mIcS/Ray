#pragma once

#include <Ray/RApplication.h>
#include <Ray/Window/RWindow.h>

#include "ExtensionLoader/ExtensionLoader.h"

namespace At0::Zeal
{

	class Sandbox : public Ray::Application
	{
	public:
		/// <summary>
		/// Sandbox Constructor
		/// </summary>
		Sandbox();

		/// <summary>
		/// Sandbox Deconstructor
		/// </summary>
		~Sandbox();


		///RAY_TODO: Currently not used! Could be used for WindowCloseEvent to check if some not MainWindow was closed which should close the entire application

		/// <summary>
		/// Receives all window events (WindowResize, WindowClose, WindowPaint)
		/// </summary>
		/// <param name="receiver">Is the window which receives the event</param>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, false otherwise</returns>
		virtual void OnEvent(Ray::Widget* receiver, Ray::Event& e);

	private:
		ExtensionLoader m_LayerLoader;
	};

}


