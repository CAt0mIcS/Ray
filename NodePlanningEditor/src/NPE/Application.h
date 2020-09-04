#pragma once

#include "Window/MainWindow.h"


namespace NPE
{
	class Button;
	class Node;

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

		void MoveNodesWithMouse(Node& node);
		void OnButtonClicked(Button& control);
		void OnNodeClicked(Node& node);
		void ResizeNodes(Node& node);

		void DrawLine(const Button& btn);
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



