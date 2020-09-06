#pragma once

#include "Window/MainWindow.h"

#include <QRD/QRD.h>


namespace NPE
{
	class Button;
	class Node;
	class TextBox;

	class Application
	{
	public:
		/**
		* Constructor for Application class
		* Sets up the database and spawns all the nodes
		*/
		Application();

		/**
		* Deleted copy-asignment functions
		*/
		Application(const Application&) = delete;
		Application operator=(const Application&) = delete;

		/**
		* Application destructor, saves scene to database
		*/
		~Application();

		/**
		* Starts the application loop
		*/
		int Run();
	
	private:
		/**
		* Receives all window events
		* 
		* @param e is the received event
		*/
		void OnEvent(const Event& e);

		/**
		* Contains logic to disallow nodes from overlapping
		* 
		* @param node is the node being draged
		*/
		void NoOverlappingNodes(Node& node);

		/**
		* Receives all button clicked events
		*
		* @param control is the button that was clicked
		*/
		void OnButtonClicked(Button& control);

		/**
		* Receives all node clicked events
		*
		* @param node is the node that was clicked
		*/
		void OnNodeClicked(Node& node);

		/**
		* Receives all textbox clicked events
		* 
		* @param txtBox is the txtBox that was clicked
		*/
		void OnTextBoxClicked(TextBox& txtBox);

		/**
		* Contains logic to drag a certain node with the mouse
		* 
		* @param node is the node to drag around
		*/
		void MoveNodesWithMouse(Node& node);

		/**
		* Contains logic for resizing the node with the mouse
		* 
		* @param node is the node to resize
		* @warning the function is unimplemented and might be stripped later
		*/
		void ResizeNodes(Node& node);

		/**
		* Contains logic for drawing lines between line buttons
		* 
		* @param btn is the start button where the line is drawn from
		*/
		void DrawLine(Button& btn);

		/**
		* Contains logic for spawning a new node when pressing Shift + A
		* 
		* @param e is the received window event
		*/
		void NewNode(const Event& e);

		/**
		* Receives all paint events
		* 
		* @param e is the received event
		*/
		void OnPaint(const Event& e);

		/**
		* Contains logic for moving all nodes with the middle mouse button
		* 
		* @param e is the received event
		*/
		void MoveNodes(const Event& e);

		/**
		* Contains logic for zooming with the scroll wheel
		* 
		* @param e is the received event
		*/
		void Zoom(const Event& e);

	private:
		MainWindow m_Window;
		
		/**
		* Records previous mouse position
		*/
		NPoint m_MousePos;
		
		QRD::Database m_Database;

		float m_Zoom;
		static constexpr float m_ZoomFactor = 0.05f;
		static constexpr float m_ResizeFactor = 1.05f;
		static constexpr float m_ZoomBoundary = 0.1f;
	};
}



