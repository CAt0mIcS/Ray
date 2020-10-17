#pragma once

#include "RlBase.h"


namespace Zeal::Reyal
{
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
		/// Virtual Application Deconstructorr
		/// </summary>
		virtual ~Application();

	protected:

	};

	/// <summary>
	/// Function which has to be defined by the client to be used in EntryPoint.h.
	/// Note: Return a raw, heap-allocated pointer because it will call delete in EntryPoint.h
	/// </summary>
	/// <returns>The newly created application</returns>
	Application* CreateApplication();

}



