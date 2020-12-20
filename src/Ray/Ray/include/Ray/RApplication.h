#pragma once

#include "RBase.h"

#include "Ray/Window/RWindow.h"
#include "Ray/Widgets/RWidget.h"

#include "Ray/Core/RThreadPool.h"
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
		// QUESTION:
		//		Should I add friend declaration and make static void Create(...) private or leave it public?
		//friend void Ray::CreateApplication();
	public:
		/// <summary>
		/// Finds a window in the stack with name
		/// </summary>
		/// <param name="name">Is the name of the window to find</param>
		/// <returns>The found window or nullptr if the window wasn't found</returns>
		Window& FindWindowByName(std::string_view name);

		/// <summary>
		/// Getter for this application, doesn't increment the std::shared_ptr reference count
		/// </summary>
		/// <returns>The current application</returns>
		static Application& Get() { return *s_Instance; }

		/// <summary>
		/// Getter for the main window
		/// </summary>
		/// <returns>The main window of this application</returns>
		Window& GetMainWindow() { return *m_MainWindow; }

		/// <summary>
		/// Getter for the main window
		/// </summary>
		/// <returns>The main window of this application</returns>
		const Window& GetMainWindow() const { return *m_MainWindow; }

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

		/// <summary>
		/// Virtual Application Deconstructorr
		/// </summary>
		virtual ~Application();

	protected:
		/// <summary>
		/// Application Constructor
		/// <param name="commandLine">Are all command line arguments from the main method</param>
		/// </summary>
		Application(std::string_view commandLine = "");

	private:
		/// <summary>
		/// Destroys resources
		/// </summary>
		void Cleanup();

		/// <summary>
		/// Does some startup stuff like allocate the console
		/// </summary>
		void StartupSetup();

	protected:
		/// <summary>
		/// The main window which will exit the application on close
		/// </summary>
		Window* m_MainWindow;

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
		/// Singelton instance of Application
		/// </summary>
		static Application* s_Instance;

		/// <summary>
		/// Specifies all threads which will read from the event queue. Create one per Window pushed
		/// </summary>
		std::vector<std::thread> m_EventLoopThreads;
	};

}


