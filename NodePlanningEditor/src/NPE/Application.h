#pragma once

#include "FileHandler.h"

#include <GUI/GUIApplication.h>

namespace GUI
{
	class PaintEvent;
	class MouseButtonPressedEvent;
	class MouseMoveEvent;
}


namespace NPE
{
	class Application : public GUI::GUIApplication
	{
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
		*/
		bool OnEvent(GUI::Control* watched, GUI::Event& e);

		/**
		* Receives all mouse move events from Application::OnEvent
		*
		* @param e is the received event
		*/
		bool OnMouseMove(GUI::MouseMoveEvent& e);

		/**
		* Receives paint event from Application::OnEvent
		* 
		* @param e is the received event
		*/
		bool OnPaintEvent(GUI::PaintEvent& e);
		
		/**
		* Receives all mouse button pressed events from Application::OnEvent
		*
		* @param e is the received event
		*/
		bool OnMouseButtonPressed(GUI::MouseButtonPressedEvent& e);

		/**
		* Application deconstructor, saves scene to file
		*/
		
		~Application() { m_FileHandler.SaveScene(this->GetWindow()); }

	private:
		FileHandler m_FileHandler;
		Util::NPoint m_MousePos;
	};
}



