#pragma once



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

	private:
		Application* m_App;
	};
}


