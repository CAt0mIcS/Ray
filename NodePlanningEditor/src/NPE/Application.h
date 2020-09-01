#pragma once

#include "Window/MainWindow.h"
#include "Handlers/Camera.h"


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
		void OnMessage();

		void MoveNodes(const Mouse::Event& e);
		void Zoom(const Mouse::Event& e);

	private:
		MainWindow m_Window;
		NPoint m_MousePos;
		Camera camera;
	};
}



