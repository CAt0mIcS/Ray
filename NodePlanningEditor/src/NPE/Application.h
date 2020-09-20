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
}


namespace NPE
{
	class Application : public GUI::GUIApplication
	{
		friend class Actions;
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
		* @param e is the received event
		* @returns true if the event was handled, false otherwise
		*/
		bool OnMouseMove(GUI::MouseMoveEvent& e);

		/**
		* Receives paint event from Application::OnEvent
		* 
		* @param e is the received event
		* @returns true if the event was handled, false otherwise
		*/
		bool OnPaintEvent(GUI::PaintEvent& e);
		
		/**
		* Receives all mouse button pressed events from Application::OnEvent
		*
		* @param e is the received event
		* @returns true if the event was handled, false otherwise
		*/
		bool OnMouseButtonPressed(GUI::MouseButtonPressedEvent& e);

		/**
		* Receives all key press events from Application::OnEvent
		* 
		* @param e is the received event
		* @returns true if the event was handled, false otherwise
		*/
		bool OnKeyPressed(GUI::KeyPressedEvent& e);

		/**
		* Application deconstructor, saves scene to file
		*/
		
		~Application() { m_FileHandler.SaveScene(this->GetWindow()); }

	private:
		FileHandler m_FileHandler;
		Actions m_Actions;
		Util::NPoint m_MousePos;

		int m_Zoom;

		static constexpr float s_NodeWidth = 450.0f;
		static constexpr float s_NodeHeight = 280.0f;
		static constexpr float s_ZoomFactor = 0.05f;
		static constexpr float s_ResizeFactor = 1.05f;
		static constexpr int s_ZoomBoundary = -45;
	};
}



