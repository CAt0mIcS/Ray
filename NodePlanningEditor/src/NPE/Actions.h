#pragma once

#include <vector>


namespace GUI
{
	class Node;
	class TextBox;
	class Control;
	class Button;
	class MainWindow;
}

namespace Util
{
	struct NPoint;
}


namespace NPE
{
	class Application;
	using Line = std::pair<GUI::Button*, GUI::Button*>;

	/**
	* Class which handles all actions (MoveNode, ResizeNode...)
	*/
	class Actions
	{
		friend class Application;
	public:
		Actions(Application& app);

		void MoveCamera(const Util::NPoint& oldMousePos, std::vector<GUI::Control*>& controls);
		void SpawnNode(GUI::MainWindow& win, float width, float height, int zoom);
		void MoveNodes(GUI::Node* node, const Util::NPoint& oldMousePos);
		void ZoomIn(int& zoom, std::vector<GUI::Control*>& controls);
		void ZoomOut(int& zoom, std::vector<GUI::Control*>& controls);
		void RenderLines(const std::vector<Line>& lines);
		void FinnishLineDrawing(std::vector<Line>& lines, const std::vector<GUI::Control*> controls);
		void EraseLine(std::vector<Line>& lines, const Util::NPoint& oldMousePos);
		void DeleteNode(GUI::Node* watched, std::vector<GUI::Control*>& controls, std::vector<Line>& lines);
		void ScrollUp(GUI::TextBox* watched);
		void ScrollDown(GUI::TextBox* watched);

	private:
		Application& m_App;

		static constexpr float s_ZoomFactor = 0.05f;
		static constexpr float s_ResizeFactor = 1.05f;
		static constexpr int s_ZoomBoundary = 45;
	};
}


