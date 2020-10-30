#ifndef RL_APPLICATION_H
#define RL_APPLICATION_H

#include "RlBase.h"

#include "Window/Window.h"
#include "Layers/LayerStack.h"

#include "Core/ThreadPool.h"

#include "Core/Queue.h"

//TODO: Separate class which contains a reference to MainWindow and implements functions that are allowed to be used

namespace At0::Reyal
{
	class Application;

	/// <summary>
	/// Function which has to be defined by the client to be used in EntryPoint.h.
	/// Note: Return a raw, heap-allocated pointer because it will call delete in EntryPoint.h
	/// </summary>
	void CreateApplication();

	class Layer;

	/// <summary>
	/// Base Application class, the client should inherit from it.
	/// </summary>
	class RL_API Application
	{
		// QUESTION:
		//		Should I add friend declaration and make static void Create(...) private or leave it public?
		//friend void Reyal::CreateApplication();
	public:
		/// <summary>
		/// Initializes the application
		/// </summary>
		/// <param name="app">Is the application</param>
		static void Create(_In_ Application* app);

		/// <summary>
		/// Deletes the application
		/// </summary>
		static void Destroy();

		/// <summary>
		/// Getter for this application, doesn't increment the std::shared_ptr reference count
		/// </summary>
		/// <returns>The current application</returns>
		static Application& Get() { return *s_Instance; }

		/// <summary>
		/// Getter for this application, increments the std::shared_ptr reference count
		/// </summary>
		/// <returns>The current application</returns>
		static Ref<Application> GetShared() { return s_Instance; }

		/// <summary>
		/// Getter for the main window
		/// </summary>
		/// <returns>The main window of this application</returns>
		Window& GetMainWindow() { return m_MainWindow; }

		/// <summary>
		/// Getter for the main window
		/// </summary>
		/// <returns>The main window of this application</returns>
		const Window& GetMainWindow() const { return m_MainWindow; }

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
		virtual void OnEvent(_In_ Widget* receiver, _In_ Event& e) = 0;

		/// <summary>
		/// Virtual Application Deconstructorr
		/// </summary>
		virtual ~Application();

	protected:
		/// <summary>
		/// Application Constructor
		/// </summary>
		Application();

	private:
		/// <summary>
		/// Receives all events from all Windows, dispatches them to the correct Layer
		/// </summary>
		/// <param name="receiver">Is the Widget that received the event</param>
		/// <param name="e">Is the received event</param>
		void OnEventReceived(_In_ Widget* receiver, Scope<Event>&& e);

		/// <summary>
		/// Calls the correct event function depending on the event
		/// </summary>
		/// <param name="layer">Is the layer which receives the event</param>
		/// <param name="receiver">Is the widget which receives the event</param>
		/// <param name="e">Is the event to dispatch</param>
		void DispatchEvent(_In_ Layer* layer, _In_ Widget* receiver, Event& e);

	protected:
		/// <summary>
		/// The main window which will exit the application on close
		/// </summary>
		Window m_MainWindow;

	private:
		LayerStack m_LayerStack;
		static Ref<Application> s_Instance;
		bool m_Running;
		ThreadPool m_ThreadPool;
	};

}

#endif /* RL_APPLICATION_H */


