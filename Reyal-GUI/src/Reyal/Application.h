#pragma once

#include "RlBase.h"

#include "Window/Window.h"
#include "Layers/LayerStack.h"

#include "Core/Queue.h"


namespace Zeal::Reyal
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
		static void Create(Application* app);

		/// <summary>
		/// Deletes the application
		/// </summary>
		static void Destroy();

		/// <summary>
		/// Getter for this application
		/// </summary>
		/// <returns>The current application</returns>
		static std::shared_ptr<Application> Get() { return s_Instance; }

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
		/// <returns>True if the event was handled, false otherwise</returns>
		bool OnEventReceived(_In_ Widget* receiver, _In_ Event& e);

	protected:
		Window m_MainWindow;

	private:
		// QUESTION: Create custom class for stack or just templated queue/stack
		LayerStack m_LayerStack;
		//Queue<Layer*> m_LayerStack;
		static std::shared_ptr<Application> s_Instance;
	};

}



