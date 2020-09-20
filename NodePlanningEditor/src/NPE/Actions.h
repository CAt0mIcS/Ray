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
	public:
		Actions(Application* app);

		void MoveCamera();
		void SpawnNode();
		void MoveNodes(GUI::Node* node);
		void ZoomIn();
		void ZoomOut();

	private:
		Application* m_App;
	};
}


