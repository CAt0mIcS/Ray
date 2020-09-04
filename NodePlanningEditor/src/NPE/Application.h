#pragma once

#include "Window/MainWindow.h"


namespace NPE
{
	class Button;

	class Application
	{
	public:
		Application();

		/**
		* Starts the application loop
		*/
		int Run();

		void OnButtonClicked(Button& control);

	private:
		void OnEvent(const Event& e);

		void DrawLine(const Button& btn);

		void Resize(const Event& e);

		void OnPaint(const Event& e);

		void MoveNodes(const Event& e);
		void Zoom(const Event& e);

	private:
		MainWindow m_Window;
		NPoint m_MousePos;
		
		char m_Zoom;
		static constexpr float m_ZoomFactor = 0.05f;
		static constexpr float m_ResizeFactor = 1.05f;
		static constexpr unsigned char m_ZoomBoundary = 40;
	};
}



