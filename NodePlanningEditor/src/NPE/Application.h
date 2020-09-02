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
		void OnMessage();

		void MoveNodes(const Mouse::Event& e);
		void Zoom(const Mouse::Event& e);

	private:
		MainWindow m_Window;
		NPoint m_MousePos;
		
		char m_Zoom;

		static constexpr float m_ZoomFactor = 0.05f;
		static constexpr float m_ResizeFactor = 1.05f;
		static constexpr unsigned char m_ZoomBoundary = 20;
	};
}



