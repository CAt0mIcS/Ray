#pragma once

#include "RBase.h"

#include "Ray/Window/RWindow.h"
#include "Ray/Core/RStack.h"


namespace At0::Ray
{
	class Application;
	class Layer;

	/// <summary>
	/// Base Application class, the client should inherit from it.
	/// </summary>
	class RAY_API Application
	{
	public:
		Window& FindWindowByName(std::string_view name);

		static Application& Get() { return *s_Instance; }
		Window& GetMainWindow() { return *m_WindowStack[0]; }
		const Window& GetMainWindow() const { return *m_WindowStack[0]; }

		/// <summary>
		/// Starts the application run loop
		/// </summary>
		/// <returns>Application exit code</returns>
		int Run();

		/// <summary>
		/// Pushes a layer onto the stack
		/// </summary>
		/// <param name="layer">Is a raw, (heap-allocated) pointer to the layer</param>
		/// <returns>The pushed layer</returns>
		Layer* PushLayer(Layer* layer);

		/// <summary>
		/// Pushes a new window onto the stack
		/// </summary>
		/// <param name="window">Is the window to push</param>
		/// <returns>The pushed window</returns>
		Window* PushWindow(Ref<Window> window);

		/// <summary>
		/// Called if any window itself received an event
		/// </summary>
		/// <param name="receiver">Is the window which received the event</param>
		/// <param name="e">Is the received event</param>
		virtual void OnEvent(Widget* receiver, Event& e) = 0;

		virtual ~Application();

	protected:
		/// <summary>
		/// Application Constructor
		/// <param name="commandLine">Are all command line arguments from the main function</param>
		/// </summary>
		Application(std::string_view commandLine = "");

	private:
		/// <summary>
		/// Destroys resources
		/// </summary>
		void Cleanup();

		/// <summary>
		/// Does some startup stuff like allocating the console
		/// </summary>
		void StartupSetup();

	private:
		/// <summary>
		/// Structure holding all Layers pushed into the application
		/// </summary>
		Stack<Layer*> m_LayerStack;

		/// <summary>
		/// Structure holding all Windows pushed into the application
		/// </summary>
		Stack<Ref<Window>> m_WindowStack;

		/// <summary>
		/// Singelton design
		/// </summary>
		static Application* s_Instance;
	};

}


