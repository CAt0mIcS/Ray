#pragma once


namespace GUI
{
	class Node;
	class TextBox;
}


namespace NPE
{
	class Application;

	/**
	* Class which handles all actions (MoveNode, ResizeNode...)
	*/
	class Actions
	{
		friend class Application;
	public:
		Actions(Application* app);

		void MoveCamera();
		void SpawnNode();
		void MoveNodes(GUI::Node* node);
		void ZoomIn();
		void ZoomOut();
		void RenderLines();
		void FinnishLineDrawing();
		void EraseLine();
		void DeleteNode(GUI::Node* watched);
		void ScrollUp(GUI::TextBox* watched);
		void ScrollDown(GUI::TextBox* watched);

	private:
		Application* m_App;

		static constexpr float s_ZoomFactor = 0.05f;
		static constexpr float s_ResizeFactor = 1.05f;
		static constexpr int s_ZoomBoundary = 45;
	};
}


