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

	/// <summary>
	/// Class which handles all actions (MoveNode, ResizeNode...)
	/// </summary>
	class Actions
	{
		friend class Application;
	public:
		/// <summary>
		/// Action Constructor
		/// </summary>
		/// <param name="app">Is the main application</param>
		Actions(Application& app);

		/// <summary>
		/// Moves the camera around
		/// </summary>
		/// <param name="diff">Is the camera movement</param>
		/// <param name="controls">Is a list of Controls to move</param>
		void MoveCamera(const Util::NPoint& diff, std::vector<GUI::Control*>& controls);

		/// <summary>
		/// Creates a new Node at mouse position
		/// </summary>
		/// <param name="win">Is the main window</param>
		/// <param name="width">Is the widht of the new Node</param>
		/// <param name="height">Is the height of the new Node</param>
		/// <param name="zoom">Is the current zoom level to size the node appropriately</param>
		void SpawnNode(GUI::MainWindow& win, float width, float height, int zoom);

		/// <summary>
		/// Moves specific Node
		/// </summary>
		/// <param name="node">Is the Node to move</param>
		/// <param name="oldMousePos">Is the cached old mouse position</param>
		void MoveNodes(GUI::Node* node, const Util::NPoint& oldMousePos);

		/// <summary>
		/// Zooms into the scene
		/// </summary>
		/// <param name="zoom">Is a reference to a variable which will hold the current zoom level</param>
		/// <param name="controls">Is the list of Controls</param>
		void ZoomIn(int& zoom, std::vector<GUI::Control*>& controls);

		/// <summary>
		/// Zooms out of the scene
		/// </summary>
		/// <param name="zoom">Is a reference to a variable which will hold the current zoom level</param>
		/// <param name="controls">Is the list of Controls</param>
		void ZoomOut(int& zoom, std::vector<GUI::Control*>& controls);

		/// <summary>
		/// Renders all lines
		/// </summary>
		/// <param name="lines">The lines to render</param>
		void RenderLines(const std::vector<Line>& lines);

		/// <summary>
		/// Connects an unfinnished line with another Button
		/// </summary>
		/// <param name="lines">Is the list of Lines where the line will be added to</param>
		/// <param name="controls">Is the list of Controls</param>
		void FinnishLineDrawing(std::vector<Line>& lines, const std::vector<GUI::Control*> controls);

		/// <summary>
		/// Deletes a line
		/// </summary>
		/// <param name="lines">Is the list of lines where the line will be removed from</param>
		/// <param name="oldMousePos">Is the cached mouse position</param>
		void EraseLine(std::vector<Line>& lines, const Util::NPoint& oldMousePos);

		/// <summary>
		/// Deletes a Node
		/// </summary>
		/// <param name="watched">Is the Node to delete</param>
		/// <param name="controls">Is the list of Controls where the Node will be removed from</param>
		/// <param name="lines">Is the list of lines where all lines connected with this Node will be removed from</param>
		void DeleteNode(GUI::Node* watched, std::vector<GUI::Control*>& controls, std::vector<Line>& lines);

		/// <summary>
		/// Scrols the text int TextBox watched
		/// </summary>
		/// <param name="watched">Is the TextBox to scroll</param>
		void ScrollUp(GUI::TextBox* watched);

		/// <summary>
		/// Scrols the text int TextBox watched
		/// </summary>
		/// <param name="watched">Is the TextBox to scroll</param>
		void ScrollDown(GUI::TextBox* watched);

	private:
		/// <summary>
		/// Reference to main Application
		/// </summary>
		Application& m_App;

		static constexpr float s_ZoomFactor = 0.05f;
		static constexpr float s_ResizeFactor = 1.05f;
		static constexpr int s_ZoomBoundary = 45;
	};
}


