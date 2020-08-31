#pragma once

#include "Window/MainWindow.h"


namespace NPE
{
	class Application
	{
	public:
		Application();

		/**
		* Starts the application loop
		*/
		int Run();

	private:
		void OnFrame();

	private:
		MainWindow m_Window;
	};
}



