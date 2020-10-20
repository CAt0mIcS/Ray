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
		/// Virtual Application Deconstructorr
		/// </summary>
		virtual ~Application();

	private:
		bool OnEvent(_In_ ZWidget* receiver, _In_ Event& e);

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



