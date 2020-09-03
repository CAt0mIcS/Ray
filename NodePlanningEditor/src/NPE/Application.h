#pragma once

#include "Window/MainWindow.h"
#include <future>

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
		void OnEvent(const Event& e);

		void MoveNodes(const Event& e);
		void Zoom(const Event& e);

	private:
		MainWindow m_Window;
		NPoint m_MousePos;
		
		std::vector<std::future<void>> m_Futures;

		char m_Zoom;
		static constexpr float m_ZoomFactor = 0.05f;
		static constexpr float m_ResizeFactor = 1.05f;
		static constexpr unsigned char m_ZoomBoundary = 20;
	};
}



