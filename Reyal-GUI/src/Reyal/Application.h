#pragma once

#include "RlBase.h"

#include "Window/Window.h"
#include "Layers/LayerStack.h"


namespace Zeal::Reyal
{
	class Layer;

	/// <summary>
	/// Base Application class, the client should inherit from it.
	/// </summary>
	class RL_API Application
	{
	public:
		/// <summary>
		/// Application Constructor
		/// </summary>
		Application();

		/// <summary>
		/// Starts the application run loop
		/// </summary>
		/// <returns>The exit code when the program terminates</returns>
		int Run();

		/// <summary>
		/// Pushes a layer onto the stack
		/// </summary>
		/// <param name="layer">Is a raw, (heap-allocated) pointer to the layer</param>
		/// <returns>The pushed layer</returns>
		void PushLayer(_In_ Layer* layer);

		/// <summary>
		/// Called if any window itself received an event
		/// </summary>
		/// <param name="receiver">Is the window which received the event</param>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, false otherwise</returns>
		virtual bool OnEvent(_In_ Widget* receiver, _In_ Event& e) = 0;

		/// <summary>
		/// Virtual Application Deconstructorr
		/// </summary>
		virtual ~Application();

	private:
		/// <summary>
		/// Receives all events from all Windows, dispatches them to the correct Layer
		/// </summary>
		/// <param name="receiver">Is the Widget that received the event</param>
		/// <param name="e">Is the received event</param>
		/// <returns>True if the event was handled, false otherwise</returns>
		bool OnEventReceived(_In_ Widget* receiver, _In_ Event& e);

	protected:
		Window m_MainWindow;

	private:
		LayerStack m_LayerStack;
	};

	/// <summary>
	/// Function which has to be defined by the client to be used in EntryPoint.h.
	/// Note: Return a raw, heap-allocated pointer because it will call delete in EntryPoint.h
	/// </summary>
	/// <returns>The newly created application</returns>
	Application* CreateApplication();

}



