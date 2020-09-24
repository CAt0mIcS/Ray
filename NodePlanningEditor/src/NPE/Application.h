#pragma once

#include "FileHandler.h"
#include "Actions.h"

#include <GUI/GUIApplication.h>

namespace GUI
{
	class PaintEvent;
	class MouseButtonPressedEvent;
	class MouseMoveEvent;
	class KeyPressedEvent;
	class MouseButtonReleasedEvent;
	class MouseWheelUpEvent;
	class MouseWheelDownEvent;

	class Button;
}


namespace NPE
{
	typedef std::pair<GUI::Button*, GUI::Button*> Line;

	class Application : public GUI::GUIApplication
	{
		friend class Actions;
		friend class FileHandler;
	public:
		/**
		* Application constructor
		*/
		Application();

		/**
		* Receives all events
		* 
		* @param watched is the control that received the event
		* @param e is the received event
		* @returns true if the event was handled, false otherwise
		*/
		bool OnEvent(GUI::Control* watched, GUI::Event& e);

		/**
		* Receives all mouse move events from Application::OnEvent
		*
		* @param watched is the control that received the event
		* @param e is the received event
		* @returns true if the event was handled, false otherwise
		*/
		bool OnMouseMove(GUI::Control* watched, GUI::MouseMoveEvent& e);

		/**
		* Receives paint event from Application::OnEvent
		* 
		* @param watched is the control that received the event
		* @param e is the received event
		* @returns true if the event was handled, false otherwise
		*/
		bool OnPaintEvent(GUI::Control* watched, GUI::PaintEvent& e);
		
		/**
		* Receives all mouse button pressed events from Application::OnEvent
		*
		* @param watched is the control that received the event
		* @param e is the received event
		* @returns true if the event was handled, false otherwise
		*/
		bool OnMouseButtonPressed(GUI::Control* watched, GUI::MouseButtonPressedEvent& e);

		/**
		* Receives all mouse button released events from Application::OnEvent
		*
		* @param watched is the control that received the event
		* @param e is the received event
		* @returns true if the event was handled, false otherwise
		*/
		bool OnMouseButtonReleased(GUI::Control* watched, GUI::MouseButtonReleasedEvent& e);

		/**
		* Receives all key press events from Application::OnEvent
		* 
		* @param watched is the control that received the event
		* @param e is the received event
		* @returns true if the event was handled, false otherwise
		*/
		bool OnKeyPressed(GUI::Control* watched, GUI::KeyPressedEvent& e);

		/**
		* Receives all mouse wheel up events from Application::OnEvent
		*
		* @param watched is the control that received the event
		* @param e is the received event
		* @returns true if the event was handled, false otherwise
		*/
		bool OnMouseWheelUp(GUI::Control* watched, GUI::MouseWheelUpEvent& e);

		/**
		* Receives all mouse wheel down events from Application::OnEvent
		*
		* @param watched is the control that received the event
		* @param e is the received event
		* @returns true if the event was handled, false otherwise
		*/
		bool OnMouseWheelDown(GUI::Control* watched, GUI::MouseWheelDownEvent& e);

		/**
		* Application deconstructor, saves scene to file
		*/
		
		~Application() { /*m_FileHandler.SaveScene(*this);*/ }

	private:

		/**
		* Contains pointers to the controls which are used by an event to work across multiple events
		*/
		struct HandleControls
		{
			GUI::Node* draggingNode;
			GUI::Button* lineButtonStart;
		};

		HandleControls m_HandleControls;
		FileHandler m_FileHandler;
		Actions m_Actions;
		
		Util::NPoint m_MousePos;
		std::vector<Line> m_Lines;
		bool m_DrawLines;

		static constexpr float s_NodeWidth = 450.0f;
		static constexpr float s_NodeHeight = 280.0f;
	};
}



