#pragma once


namespace GUI
{
	class Node;
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
		void DrawLine();
		void RenderLines();

	private:
		Application* m_App;

		int m_Zoom;
		static constexpr float s_ZoomFactor = 0.05f;
		static constexpr float s_ResizeFactor = 1.05f;
		static constexpr int s_ZoomBoundary = 45;
	};
}


